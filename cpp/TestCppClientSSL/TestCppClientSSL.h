/* Copyright (C) 2017 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef testcppclient_h__INCLUDED
#define testcppclient_h__INCLUDED

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "../ssl/EReaderSSL.h"

#include <memory>

class EClientSocketSSL;

enum State {
	ST_CONNECT,
	ST_PLACEORDER,
	ST_PLACEORDER_ACK,
	ST_CANCELORDER,
	ST_CANCELORDER_ACK,
	ST_PING,
	ST_PING_ACK,
	ST_IDLE
};


class TestCppClient : public EWrapper
{
public:

	TestCppClient();
	~TestCppClient();

	void setConnectOptions(const std::string&);
	void processMessages();

public:

	bool connect(const char * host, unsigned int port, int clientId = 0);
	void disconnect() const;
	bool isConnected() const;

private:

	void reqCurrentTime();
	void placeOrder();
	void cancelOrder();

public:
	// events
	void tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attrib);
	void tickSize(TickerId tickerId, TickType field, int size);
	void tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
		double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice);
	void tickGeneric(TickerId tickerId, TickType tickType, double value);
	void tickString(TickerId tickerId, TickType tickType, const std::string& value);
	void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
		double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate);
	void orderStatus(OrderId orderId, const std::string& status, double filled,
		double remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice);
	void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&);
	void openOrderEnd();
	void winError(const std::string& str, int lastError);
	void connectionClosed();
	void updateAccountValue(const std::string& key, const std::string& val,
		const std::string& currency, const std::string& accountName);
	void updatePortfolio(const Contract& contract, double position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const std::string& accountName);
	void updateAccountTime(const std::string& timeStamp);
	void accountDownloadEnd(const std::string& accountName);
	void nextValidId(OrderId orderId);
	void contractDetails(int reqId, const ContractDetails& contractDetails);
	void bondContractDetails(int reqId, const ContractDetails& contractDetails);
	void contractDetailsEnd(int reqId);
	void execDetails(int reqId, const Contract& contract, const Execution& execution);
	void execDetailsEnd(int reqId);
	void error(const int id, const int errorCode, const std::string errorString);
	void updateMktDepth(TickerId id, int position, int operation, int side,
		double price, int size);
	void updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
		int side, double price, int size);
	void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch);
	void managedAccounts(const std::string& accountsList);
	void receiveFA(faDataType pFaDataType, const std::string& cxml);
	void historicalData(TickerId reqId, Bar bar);
	void historicalDataEnd(int reqId, std::string startDateStr, std::string endDateStr);
	void scannerParameters(const std::string& xml);
	void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
		const std::string& distance, const std::string& benchmark, const std::string& projection,
		const std::string& legsStr);
	void scannerDataEnd(int reqId);
	void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
		long volume, double wap, int count);
	void currentTime(long time);
	void fundamentalData(TickerId reqId, const std::string& data);
	void deltaNeutralValidation(int reqId, const UnderComp& underComp);
	void tickSnapshotEnd(int reqId);
	void marketDataType(TickerId reqId, int marketDataType);
	void commissionReport( const CommissionReport& commissionReport);
	void position( const std::string& account, const Contract& contract, double position, double avgCost);
	void positionEnd();
	void accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency);
	void accountSummaryEnd( int reqId);
	void verifyMessageAPI( const std::string& apiData);
	void verifyCompleted( bool isSuccessful, const std::string& errorText);
	void verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallenge);
	void verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText);
	void displayGroupList( int reqId, const std::string& groups);
	void displayGroupUpdated( int reqId, const std::string& contractInfo);
    void connectAck();
	void positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost);
	void positionMultiEnd( int reqId);
	void accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency);
	void accountUpdateMultiEnd( int reqId);
	void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes);
	void securityDefinitionOptionalParameterEnd(int reqId);
	void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers);
	void familyCodes(const std::vector<FamilyCode> &familyCodes);
	void symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions);
	void mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions);
	void tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData);
    void smartComponents(int reqId, SmartComponentsMap theMap);
    void tickReqParams(int tickerId, double minTick, std::string bboExchange, int snapshotPermissions);
	void newsProviders(const std::vector<NewsProvider> &newsProviders);
	void newsArticle(int requestId, int articleType, const std::string& articleText);
	void historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline);
	void historicalNewsEnd(int requestId, bool hasMore);
	void headTimestamp(int reqId, const std::string& headTimestamp);
	void histogramData(int reqId, HistogramDataVector data);
    void historicalDataUpdate(TickerId reqId, Bar bar);
    void rerouteMktDataReq(int reqId, int conid, const std::string& exchange);
    void rerouteMktDepthReq(int reqId, int conid, const std::string& exchange);
    void marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements);
    void dailyPnL(int reqId, double dailyPnL);
    void dailyPnLSingle(int reqId, int pos, double dailyPnL, double value);
    void pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL);
    void pnlSingle(int reqId, int pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value);
    void historicalTicks(int reqId, const std::vector<HistoricalTick>& ticks, bool done);
    void historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk>& ticks, bool done);
    void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done);
    void tickByTickAllLast(int reqId, int tickType, time_t time, double price, int size, const TickAttrib& attribs, const std::string& exchange, const std::string& specialConditions);
    void tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, int bidSize, int askSize, const TickAttrib& attribs);
    void tickByTickMidPoint(int reqId, time_t time, double midPoint);

private:
    EReaderOSSignal m_osSignal;
    EClientSocketSSL * const m_pClient;
	State m_state;
	time_t m_sleepDeadline;

	OrderId m_orderId;
	EReaderSSL *m_pReader;
	
    bool m_extraAuth;
};

#endif

