/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"

#include "TestCppClientSSL.h"

#include "../ssl/EClientSocketSSL.h"
#include "EPosixClientSocketPlatform.h"

#include "Contract.h"
#include "Order.h"

#include <stdio.h>

const int PING_DEADLINE = 2; // seconds
const int SLEEP_BETWEEN_PINGS = 30; // seconds

///////////////////////////////////////////////////////////
// member funcs
TestCppClient::TestCppClient() : 
      m_osSignal(2000)//2-seconds timeout
    , m_pClient(new EClientSocketSSL(this, &m_osSignal))
	, m_state(ST_CONNECT)
	, m_sleepDeadline(0)
	, m_orderId(0)
    , m_pReader(0)
    , m_extraAuth(false)
{
}

TestCppClient::~TestCppClient()
{
    if (m_pReader)
        delete m_pReader;

    delete m_pClient;
}

bool TestCppClient::connect(const char *host, unsigned int port, int clientId)
{
	// trying to connect
	printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);

	bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);

	if (bRes) {
		printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

        m_pReader = new EReaderSSL(m_pClient, &m_osSignal);
	}
	else
		printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

	return bRes;
}

void TestCppClient::disconnect() const
{
	m_pClient->eDisconnect();

	printf ( "Disconnected\n");
}

bool TestCppClient::isConnected() const
{
	return m_pClient->isConnected();
}

void TestCppClient::setConnectOptions(const std::string& connectOptions)
{
	m_pClient->setConnectOptions(connectOptions);
}

void TestCppClient::processMessages() {
	fd_set readSet, writeSet, errorSet;

	struct timeval tval;
	tval.tv_usec = 0;
	tval.tv_sec = 0;

	time_t now = time(NULL);

	switch (m_state) {
		case ST_PLACEORDER:
			placeOrder();
			break;
		case ST_PLACEORDER_ACK:
			break;
		case ST_CANCELORDER:
			cancelOrder();
			break;
		case ST_CANCELORDER_ACK:
			break;
		case ST_PING:
			reqCurrentTime();
			break;
		case ST_PING_ACK:
			if( m_sleepDeadline < now) {
				disconnect();
				return;
			}
			break;
		case ST_IDLE:
			if( m_sleepDeadline < now) {
				m_state = ST_PING;
				return;
			}
			break;
	}

	if( m_sleepDeadline > 0) {
		// initialize timeout with m_sleepDeadline - now
		tval.tv_sec = m_sleepDeadline - now;
	}

	m_pReader->checkClient();
	m_osSignal.waitForSignal();
	m_pReader->processMsgs();
}

//////////////////////////////////////////////////////////////////
// methods
void TestCppClient::connectAck() {
	if (!m_extraAuth && m_pClient->asyncEConnect())
        m_pClient->startApi();
}

void TestCppClient::reqCurrentTime()
{
	printf( "Requesting Current Time\n");

	// set ping deadline to "now + n seconds"
	m_sleepDeadline = time( NULL) + PING_DEADLINE;

	m_state = ST_PING_ACK;

	m_pClient->reqCurrentTime();
}

void TestCppClient::placeOrder()
{
	Contract contract;
	Order order;

	contract.symbol = "IBM";
	contract.secType = "STK";
	contract.exchange = "SMART";
	contract.currency = "USD";

	order.action = "BUY";
	order.totalQuantity = 1000;
	order.orderType = "LMT";
	order.lmtPrice = 0.01;

	printf( "Placing Order %ld: %s %f %s at %f\n", ++m_orderId, order.action.c_str(), order.totalQuantity, contract.symbol.c_str(), order.lmtPrice);

	m_state = ST_PLACEORDER_ACK;

	m_pClient->placeOrder( m_orderId, contract, order);
}

void TestCppClient::cancelOrder()
{
	printf( "Cancelling Order %ld\n", m_orderId);

	m_state = ST_CANCELORDER_ACK;

	m_pClient->cancelOrder( m_orderId);
}

///////////////////////////////////////////////////////////////////
// events
void TestCppClient::orderStatus( OrderId orderId, const std::string& status, double filled,
	                            double remaining, double avgFillPrice, int permId, int parentId,
	                            double lastFillPrice, int clientId, const std::string& whyHeld)

{
	if( orderId == m_orderId) {
		if( m_state == ST_PLACEORDER_ACK && (status == "PreSubmitted" || status == "Submitted"))
			m_state = ST_CANCELORDER;

		if( m_state == ST_CANCELORDER_ACK && status == "Cancelled")
			m_state = ST_PING;

		printf( "Order: id=%ld, status=%s\n", orderId, status.c_str());
	}
}

void TestCppClient::nextValidId( OrderId orderId)
{
	m_orderId = orderId;

	m_state = ST_PLACEORDER;
}

void TestCppClient::currentTime( long time)
{
	if ( m_state == ST_PING_ACK) {
		time_t t = ( time_t)time;
		struct tm * timeinfo = localtime ( &t);
		printf( "The current date/time is: %s", asctime( timeinfo));

		time_t now = ::time(NULL);
		m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;

		m_state = ST_IDLE;
	}
}

void TestCppClient::error(const int id, const int errorCode, const std::string errorString)
{
	//  printf( "Error id=%d, errorCode=%d, msg=%s\n", id, errorCode, errorString.c_str());

	if( id == -1 && errorCode == 1100) // if "Connectivity between IB and TWS has been lost"
		disconnect();
}

void TestCppClient::tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute) {}
void TestCppClient::tickSize( TickerId tickerId, TickType field, int size) {}
void TestCppClient::tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                          double optPrice, double pvDividend,
                                          double gamma, double vega, double theta, double undPrice) {}
void TestCppClient::tickGeneric(TickerId tickerId, TickType tickType, double value) {}
void TestCppClient::tickString(TickerId tickerId, TickType tickType, const std::string& value) {}
void TestCppClient::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
                            double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {}
void TestCppClient::openOrder( OrderId orderId, const Contract&, const Order&, const OrderState& ostate) {}
void TestCppClient::openOrderEnd() {}
void TestCppClient::winError( const std::string& str, int lastError) {}
void TestCppClient::connectionClosed() {}
void TestCppClient::updateAccountValue(const std::string& key, const std::string& val,
                                       const std::string& currency, const std::string& accountName) {}
void TestCppClient::updatePortfolio(const Contract& contract, double position,
                                    double marketPrice, double marketValue, double averageCost,
                                    double unrealizedPNL, double realizedPNL, const std::string& accountName){}
void TestCppClient::updateAccountTime(const std::string& timeStamp) {}
void TestCppClient::accountDownloadEnd(const std::string& accountName) {}
void TestCppClient::contractDetails( int reqId, const ContractDetails& contractDetails) {}
void TestCppClient::bondContractDetails( int reqId, const ContractDetails& contractDetails) {}
void TestCppClient::contractDetailsEnd( int reqId) {}
void TestCppClient::execDetails( int reqId, const Contract& contract, const Execution& execution) {}
void TestCppClient::execDetailsEnd( int reqId) {}

void TestCppClient::updateMktDepth(TickerId id, int position, int operation, int side,
                                   double price, int size) {}
void TestCppClient::updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                                     int side, double price, int size) {}
void TestCppClient::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) {}
void TestCppClient::managedAccounts( const std::string& accountsList) {}
void TestCppClient::receiveFA(faDataType pFaDataType, const std::string& cxml) {}
void TestCppClient::historicalData(TickerId reqId, const std::string& date, double open, double high,
                                   double low, double close, int volume, int barCount, double WAP, int hasGaps) {}
void TestCppClient::scannerParameters(const std::string& xml) {}
void TestCppClient::scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                                const std::string& distance, const std::string& benchmark, const std::string& projection,
                                const std::string& legsStr) {}
void TestCppClient::scannerDataEnd(int reqId) {}
void TestCppClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                                long volume, double wap, int count) {}
void TestCppClient::fundamentalData(TickerId reqId, const std::string& data) {}
void TestCppClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
void TestCppClient::tickSnapshotEnd(int reqId) {}
void TestCppClient::marketDataType(TickerId reqId, int marketDataType) {}
void TestCppClient::commissionReport( const CommissionReport& commissionReport) {}
void TestCppClient::position( const std::string& account, const Contract& contract, double position, double avgCost) {}
void TestCppClient::positionEnd() {}
void TestCppClient::accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency) {}
void TestCppClient::accountSummaryEnd( int reqId) {}
void TestCppClient::verifyMessageAPI( const std::string& apiData) {}
void TestCppClient::verifyCompleted( bool isSuccessful, const std::string& errorText) {}
void TestCppClient::verifyAndAuthMessageAPI( const std::string& apiDatai, const std::string& xyzChallenge) {}

void TestCppClient::verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) {
    if (isSuccessful)
        m_pClient->startApi();
}

void TestCppClient::displayGroupList( int reqId, const std::string& groups) {}
void TestCppClient::displayGroupUpdated( int reqId, const std::string& contractInfo) {}
void TestCppClient::positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost) {}
void TestCppClient::positionMultiEnd( int reqId) {}
void TestCppClient::accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) {}
void TestCppClient::accountUpdateMultiEnd( int reqId) {}
void TestCppClient::securityDefinitionOptionalParameter(int reqId, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes) {}
void TestCppClient::securityDefinitionOptionalParameterEnd(int reqId) {}
