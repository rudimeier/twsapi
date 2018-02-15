/* Copyright (C) 2017 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"

#include "TestCppClient.h"

#include "EClientSocket.h"
#include "EPosixClientSocketPlatform.h"

#include "Contract.h"
#include "Order.h"
#include "OrderState.h"
#include "Execution.h"
#include "CommissionReport.h"
#include "ContractSamples.h"
#include "OrderSamples.h"
#include "ScannerSubscription.h"
#include "ScannerSubscriptionSamples.h"
#include "executioncondition.h"
#include "PriceCondition.h"
#include "MarginCondition.h"
#include "PercentChangeCondition.h"
#include "TimeCondition.h"
#include "VolumeCondition.h"
#include "AvailableAlgoParams.h"
#include "FAMethodSamples.h"
#include "CommonDefs.h"
#include "AccountSummaryTags.h"

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <ctime>

const int PING_DEADLINE = 2; // seconds
const int SLEEP_BETWEEN_PINGS = 30; // seconds

///////////////////////////////////////////////////////////
// member funcs
//! [socket_init]
TestCppClient::TestCppClient() :
      m_osSignal(2000)//2-seconds timeout
    , m_pClient(new EClientSocket(this, &m_osSignal))
	, m_state(ST_CONNECT)
	, m_sleepDeadline(0)
	, m_orderId(0)
    , m_pReader(0)
    , m_extraAuth(false)
{
}
//! [socket_init]
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
	
	//! [connect]
	bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);
	//! [connect]
	
	if (bRes) {
		printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
		//! [ereader]
        m_pReader = new EReader(m_pClient, &m_osSignal);
		m_pReader->start();
		//! [ereader]
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
	struct timeval tval;
	tval.tv_usec = 0;
	tval.tv_sec = 0;

	time_t now = time(NULL);

	/*****************************************************************/
    /* Below are few quick-to-test examples on the IB API functions grouped by functionality. Uncomment the relevant methods. */
    /*****************************************************************/
	switch (m_state) {
		case ST_PNLSINGLE:
			pnlSingleOperation();
			break;
		case ST_PNLSINGLE_ACK:
			break;
		case ST_PNL:
			pnlOperation();
			break;
		case ST_PNL_ACK:
			break;
		case ST_TICKDATAOPERATION:
			tickDataOperation();
			break;
		case ST_TICKDATAOPERATION_ACK:
			break;
		case ST_DELAYEDTICKDATAOPERATION:
			delayedTickDataOperation();
			break;
		case ST_DELAYEDTICKDATAOPERATION_ACK:
			break;
		case ST_MARKETDEPTHOPERATION:
			marketDepthOperations();
			break;
		case ST_MARKETDEPTHOPERATION_ACK:
			break;
		case ST_REALTIMEBARS:
			realTimeBars();
			break;
		case ST_REALTIMEBARS_ACK:
			break;
		case ST_MARKETDATATYPE:
			marketDataType();
			break;
		case ST_MARKETDATATYPE_ACK:
			break;
		case ST_HISTORICALDATAREQUESTS:
			historicalDataRequests();
			break;
		case ST_HISTORICALDATAREQUESTS_ACK:
			break;
		case ST_OPTIONSOPERATIONS:
			optionsOperations();
			break;
		case ST_OPTIONSOPERATIONS_ACK:
			break;
		case ST_CONTRACTOPERATION:
			contractOperations();
			break;
		case ST_CONTRACTOPERATION_ACK:
			break;
		case ST_MARKETSCANNERS:
			marketScanners();
			break;
		case ST_MARKETSCANNERS_ACK:
			break;
		case ST_REUTERSFUNDAMENTALS:
			reutersFundamentals();
			break;
		case ST_REUTERSFUNDAMENTALS_ACK:
			break;
		case ST_BULLETINS:
			bulletins();
			break;
		case ST_BULLETINS_ACK:
			break;
		case ST_ACCOUNTOPERATIONS:
			accountOperations();
			break;
		case ST_ACCOUNTOPERATIONS_ACK:
			break;
		case ST_ORDEROPERATIONS:
			orderOperations();
			break;
		case ST_ORDEROPERATIONS_ACK:
			break;
		case ST_OCASAMPLES:
			ocaSamples();
			break;
		case ST_OCASAMPLES_ACK:
			break;
		case ST_CONDITIONSAMPLES:
			conditionSamples();
			break;
		case ST_CONDITIONSAMPLES_ACK:
			break;
		case ST_BRACKETSAMPLES:
			bracketSample();
			break;
		case ST_BRACKETSAMPLES_ACK:
			break;
		case ST_HEDGESAMPLES:
			hedgeSample();
			break;
		case ST_HEDGESAMPLES_ACK:
			break;
		case ST_TESTALGOSAMPLES:
			testAlgoSamples();
			break;
		case ST_TESTALGOSAMPLES_ACK:
			break;
		case ST_FAORDERSAMPLES:
			financialAdvisorOrderSamples();
			break;
		case ST_FAORDERSAMPLES_ACK:
			break;
		case ST_FAOPERATIONS:
			financialAdvisorOperations();
			break;
		case ST_FAOPERATIONS_ACK:
			break;
		case ST_DISPLAYGROUPS:
			testDisplayGroups();
			break;
		case ST_DISPLAYGROUPS_ACK:
			break;
		case ST_MISCELANEOUS:
			miscelaneous();
			break;
		case ST_MISCELANEOUS_ACK:
			break;
		case ST_FAMILYCODES:
			reqFamilyCodes();
			break;
		case ST_FAMILYCODES_ACK:
			break;
		case ST_SYMBOLSAMPLES:
			reqMatchingSymbols();
			break;
		case ST_SYMBOLSAMPLES_ACK:
			break;
		case ST_REQMKTDEPTHEXCHANGES:
			reqMktDepthExchanges();
			break;
		case ST_REQMKTDEPTHEXCHANGES_ACK:
			break;
		case ST_REQNEWSTICKS:
			reqNewsTicks();
			break;
		case ST_REQNEWSTICKS_ACK:
			break;
		case ST_REQSMARTCOMPONENTS:
			reqSmartComponents();
			break;
		case ST_REQSMARTCOMPONENTS_ACK:
			break;
		case ST_NEWSPROVIDERS:
			reqNewsProviders();
			break;
		case ST_NEWSPROVIDERS_ACK:
			break;
		case ST_REQNEWSARTICLE:
			reqNewsArticle();
			break;
		case ST_REQNEWSARTICLE_ACK:
			break;
		case ST_REQHISTORICALNEWS:
			reqHistoricalNews();
			break;
		case ST_REQHISTORICALNEWS_ACK:
			break;
		case ST_REQHEADTIMESTAMP:
			reqHeadTimestamp();
			break;
		case ST_REQHISTOGRAMDATA:
			reqHistogramData();
			break;
		case ST_REROUTECFD:
			rerouteCFDOperations();
			break;
		case ST_MARKETRULE:
			marketRuleOperations();
			break;
		case ST_CONTFUT:
			continuousFuturesOperations();
			break;
        case ST_REQHISTORICALTICKS:
            reqHistoricalTicks();
            break;
        case ST_REQHISTORICALTICKS_ACK:
            break;
		case ST_REQTICKBYTICKDATA:
			reqTickByTickData();
			break;
		case ST_REQTICKBYTICKDATA_ACK:
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
	errno = 0;
	m_pReader->processMsgs();
}

//////////////////////////////////////////////////////////////////
// methods
//! [connectack]
void TestCppClient::connectAck() {
	if (!m_extraAuth && m_pClient->asyncEConnect())
        m_pClient->startApi();
}
//! [connectack]

void TestCppClient::reqCurrentTime()
{
	printf( "Requesting Current Time\n");

	// set ping deadline to "now + n seconds"
	m_sleepDeadline = time( NULL) + PING_DEADLINE;

	m_state = ST_PING_ACK;

	m_pClient->reqCurrentTime();
}

void TestCppClient::pnlOperation()
{
	//! [reqpnl]
    m_pClient->reqPnL(7001, "DUD00029", "");
	//! [reqpnl]
	
    std::this_thread::sleep_for(std::chrono::seconds(2));

	//! [cancelpnl]
    m_pClient->cancelPnL(7001);
	//! [cancelpnl] 
	
    m_state = ST_PNL_ACK;
}

void TestCppClient::pnlSingleOperation()
{
	//! [reqpnlsingle]
    m_pClient->reqPnLSingle(7002, "DUD00029", "", 268084);
	//! [reqpnlsingle]
	
    std::this_thread::sleep_for(std::chrono::seconds(2));

	//! [cancelpnlsingle]
    m_pClient->cancelPnLSingle(7002);
	//! [cancelpnlsingle]
	
    m_state = ST_PNLSINGLE_ACK;
}

void TestCppClient::tickDataOperation()
{
	/*** Requesting real time market data ***/
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //! [reqmktdata]
	m_pClient->reqMktData(1001, ContractSamples::StockComboContract(), "", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1002, ContractSamples::OptionWithLocalSymbol(), "", false, false, TagValueListSPtr());
	//! [reqmktdata]
	//! [reqmktdata_snapshot]
	m_pClient->reqMktData(1003, ContractSamples::FutureComboContract(), "", true, false, TagValueListSPtr());
	//! [reqmktdata_snapshot]

	/*
	//! [regulatorysnapshot]
	// Each regulatory snapshot incurs a fee of 0.01 USD
	m_pClient->reqMktData(1013, ContractSamples::USStock(), "", false, true, TagValueListSPtr());
	//! [regulatorysnapshot]
	*/
	
	//! [reqmktdata_genticks]
	//Requesting RTVolume (Time & Sales), shortable and Fundamental Ratios generic ticks
	m_pClient->reqMktData(1004, ContractSamples::USStock(), "233,236,258", false, false, TagValueListSPtr());
	//! [reqmktdata_genticks]

	//! [reqmktdata_contractnews]
	// Without the API news subscription this will generate an "invalid tick type" error
	m_pClient->reqMktData(1005, ContractSamples::USStock(), "mdoff,292:BZ", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1006, ContractSamples::USStock(), "mdoff,292:BT", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1007, ContractSamples::USStock(), "mdoff,292:FLY", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1008, ContractSamples::USStock(), "mdoff,292:MT", false, false, TagValueListSPtr());
	//! [reqmktdata_contractnews]
	//! [reqmktdata_broadtapenews]
	m_pClient->reqMktData(1009, ContractSamples::BTbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1010, ContractSamples::BZbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1011, ContractSamples::FLYbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1012, ContractSamples::MTbroadtapeNewsFeed(), "mdoff,292", false, false, TagValueListSPtr());
	//! [reqmktdata_broadtapenews]

	//! [reqoptiondatagenticks]
	//Requesting data for an option contract will return the greek values
	m_pClient->reqMktData(1005, ContractSamples::USOptionContract(), "", false, false, TagValueListSPtr());
	//! [reqoptiondatagenticks]
	
	//! [reqfuturesopeninterest]
	//Requesting data for a futures contract will return the futures open interest
	m_pClient->reqMktData(1014, ContractSamples::SimpleFuture(), "mdoff,588", false, false, TagValueListSPtr());
	//! [reqfuturesopeninterest]

	//! [reqpreopenbidask]
	//Requesting data for a futures contract will return the pre-open bid/ask flag
	m_pClient->reqMktData(1015, ContractSamples::SimpleFuture(), "", false, false, TagValueListSPtr());
	//! [reqpreopenbidask]

	std::this_thread::sleep_for(std::chrono::seconds(1));
	/*** Canceling the market data subscription ***/
	//! [cancelmktdata]
	m_pClient->cancelMktData(1001);
	m_pClient->cancelMktData(1002);
	m_pClient->cancelMktData(1003);
	m_pClient->cancelMktData(1014);
	m_pClient->cancelMktData(1015);
	//! [cancelmktdata]

	m_state = ST_TICKDATAOPERATION_ACK;
}

void TestCppClient::delayedTickDataOperation()
{
	/*** Requesting delayed market data ***/

	//! [reqmktdata_delayedmd]
	m_pClient->reqMarketDataType(4); // send delayed-frozen (4) market data type
	m_pClient->reqMktData(1013, ContractSamples::HKStk(), "", false, false, TagValueListSPtr());
	m_pClient->reqMktData(1014, ContractSamples::USOptionContract(), "", false, false, TagValueListSPtr());
	//! [reqmktdata_delayedmd]

	std::this_thread::sleep_for(std::chrono::seconds(10));

	/*** Canceling the delayed market data subscription ***/
	//! [cancelmktdata_delayedmd]
	m_pClient->cancelMktData(1013);
	m_pClient->cancelMktData(1014);
	//! [cancelmktdata_delayedmd]

	m_state = ST_DELAYEDTICKDATAOPERATION_ACK;
}

void TestCppClient::marketDepthOperations()
{
	/*** Requesting the Deep Book ***/
	//! [reqmarketdepth]
	m_pClient->reqMktDepth(2001, ContractSamples::EurGbpFx(), 5, TagValueListSPtr());
	//! [reqmarketdepth]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Canceling the Deep Book request ***/
	//! [cancelmktdepth]
	m_pClient->cancelMktDepth(2001);
	//! [cancelmktdepth]

	m_state = ST_MARKETDEPTHOPERATION_ACK;
}

void TestCppClient::realTimeBars()
{
	/*** Requesting real time bars ***/
	//! [reqrealtimebars]
	m_pClient->reqRealTimeBars(3001, ContractSamples::EurGbpFx(), 5, "MIDPOINT", true, TagValueListSPtr());
	//! [reqrealtimebars]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Canceling real time bars ***/
    //! [cancelrealtimebars]
	m_pClient->cancelRealTimeBars(3001);
    //! [cancelrealtimebars]

	m_state = ST_REALTIMEBARS_ACK;
}

void TestCppClient::marketDataType()
{
	//! [reqmarketdatatype]
	/*** By default only real-time (1) market data is enabled
		 Sending frozen (2) enables frozen market data
		 Sending delayed (3) enables delayed market data and disables delayed-frozen market data
		 Sending delayed-frozen (4) enables delayed and delayed-frozen market data
		 Sending real-time (1) disables frozen, delayed and delayed-frozen market data ***/
	m_pClient->reqMarketDataType(2);
	//! [reqmarketdatatype]

	m_state = ST_MARKETDATATYPE_ACK;
}

void TestCppClient::historicalDataRequests()
{
	/*** Requesting historical data ***/
	//! [reqhistoricaldata]
	std::time_t rawtime;
    std::tm* timeinfo;
    char queryTime [80];

	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
	std::strftime(queryTime, 80, "%Y%m%d %H:%M:%S", timeinfo);

	m_pClient->reqHistoricalData(4001, ContractSamples::EurGbpFx(), queryTime, "1 M", "1 day", "MIDPOINT", 1, 1, false, TagValueListSPtr());
	m_pClient->reqHistoricalData(4002, ContractSamples::EuropeanStock(), queryTime, "10 D", "1 min", "TRADES", 1, 1, false, TagValueListSPtr());
	//! [reqhistoricaldata]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Canceling historical data requests ***/
	m_pClient->cancelHistoricalData(4001);
	m_pClient->cancelHistoricalData(4002);

	m_state = ST_HISTORICALDATAREQUESTS_ACK;
}

void TestCppClient::optionsOperations()
{
	//! [reqsecdefoptparams]
	m_pClient->reqSecDefOptParams(0, "IBM", "", "STK", 8314);
	//! [reqsecdefoptparams]

	//! [calculateimpliedvolatility]
	m_pClient->calculateImpliedVolatility(5001, ContractSamples::NormalOption(), 5, 85);
	//! [calculateimpliedvolatility]

	//** Canceling implied volatility ***
	m_pClient->cancelCalculateImpliedVolatility(5001);

	//! [calculateoptionprice]
	m_pClient->calculateOptionPrice(5002, ContractSamples::NormalOption(), 0.22, 85);
	//! [calculateoptionprice]

	//** Canceling option's price calculation ***
	m_pClient->cancelCalculateOptionPrice(5002);

	//! [exercise_options]
	//** Exercising options ***
	m_pClient->exerciseOptions(5003, ContractSamples::OptionWithTradingClass(), 1, 1, "", 1);
	//! [exercise_options]

	m_state = ST_OPTIONSOPERATIONS_ACK;
}

void TestCppClient::contractOperations()
{
	m_pClient->reqContractDetails(209, ContractSamples::EurGbpFx());
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [reqcontractdetails]
	m_pClient->reqContractDetails(210, ContractSamples::OptionForQuery());
	m_pClient->reqContractDetails(212, ContractSamples::IBMBond());
	m_pClient->reqContractDetails(213, ContractSamples::IBKRStk());
	m_pClient->reqContractDetails(214, ContractSamples::Bond());
	m_pClient->reqContractDetails(215, ContractSamples::FuturesOnOptions());
	//! [reqcontractdetails]

	//! [reqcontractdetailsnews]
	m_pClient->reqContractDetails(211, ContractSamples::NewsFeedForQuery());
	//! [reqcontractdetailsnews]

	m_state = ST_CONTRACTOPERATION_ACK;
}

void TestCppClient::marketScanners()
{
	/*** Requesting all available parameters which can be used to build a scanner request ***/
	//! [reqscannerparameters]
	m_pClient->reqScannerParameters();
	//! [reqscannerparameters]
	std::this_thread::sleep_for(std::chrono::seconds(2));

	/*** Triggering a scanner subscription ***/
	//! [reqscannersubscription]
	m_pClient->reqScannerSubscription(7001, ScannerSubscriptionSamples::HotUSStkByVolume(), TagValueListSPtr());
	//! [reqscannersubscription]

	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Canceling the scanner subscription ***/
	//! [cancelscannersubscription]
	m_pClient->cancelScannerSubscription(7001);
	//! [cancelscannersubscription]

	m_state = ST_MARKETSCANNERS_ACK;
}

void TestCppClient::reutersFundamentals()
{
	/*** Requesting Fundamentals ***/
	//! [reqfundamentaldata]
	m_pClient->reqFundamentalData(8001, ContractSamples::USStock(), "ReportsFinSummary");
	//! [reqfundamentaldata]
	std::this_thread::sleep_for(std::chrono::seconds(2));

	/*** Canceling fundamentals request ***/
	//! [cancelfundamentaldata]
	m_pClient->cancelFundamentalData(8001);
	//! [cancelfundamentaldata]

	m_state = ST_REUTERSFUNDAMENTALS_ACK;
}

void TestCppClient::bulletins()
{
	/*** Requesting Interactive Broker's news bulletins */
	//! [reqnewsbulletins]
	m_pClient->reqNewsBulletins(true);
	//! [reqnewsbulletins]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Canceling IB's news bulletins ***/
	//! [cancelnewsbulletins]
	m_pClient->cancelNewsBulletins();
	//! [cancelnewsbulletins]

	m_state = ST_BULLETINS_ACK;
}

void TestCppClient::accountOperations()
{
	/*** Requesting managed accounts***/
	//! [reqmanagedaccts]
	m_pClient->reqManagedAccts();
	//! [reqmanagedaccts]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Requesting accounts' summary ***/
	//! [reqaaccountsummary]
	m_pClient->reqAccountSummary(9001, "All", AccountSummaryTags::getAllTags());
	//! [reqaaccountsummary]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [reqaaccountsummaryledger]
	m_pClient->reqAccountSummary(9002, "All", "$LEDGER");
	//! [reqaaccountsummaryledger]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [reqaaccountsummaryledgercurrency]
	m_pClient->reqAccountSummary(9003, "All", "$LEDGER:EUR");
	//! [reqaaccountsummaryledgercurrency]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [reqaaccountsummaryledgerall]
	m_pClient->reqAccountSummary(9004, "All", "$LEDGER:ALL");
	//! [reqaaccountsummaryledgerall]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [cancelaaccountsummary]
	m_pClient->cancelAccountSummary(9001);
	m_pClient->cancelAccountSummary(9002);
	m_pClient->cancelAccountSummary(9003);
	m_pClient->cancelAccountSummary(9004);
	//! [cancelaaccountsummary]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	/*** Subscribing to an account's information. Only one at a time! ***/
	//! [reqaaccountupdates]
	m_pClient->reqAccountUpdates(true, "U150462");
	//! [reqaaccountupdates]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [cancelaaccountupdates]
	m_pClient->reqAccountUpdates(false, "U150462");
	//! [cancelaaccountupdates]
	std::this_thread::sleep_for(std::chrono::seconds(2));

	//! [reqaaccountupdatesmulti]
	m_pClient->reqAccountUpdatessMulti(9002, "U150462", "EUstocks", true);
	//! [reqaaccountupdatesmulti]
	std::this_thread::sleep_for(std::chrono::seconds(2));

	/*** Requesting all accounts' positions. ***/
	//! [reqpositions]
	m_pClient->reqPositions();
	//! [reqpositions]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [cancelpositions]
	m_pClient->cancelPositions();
	//! [cancelpositions]

	//! [reqpositionsmulti]
	m_pClient->reqPositionsMulti(9003, "U150462", "EUstocks");
	//! [reqpositionsmulti]

	m_state = ST_ACCOUNTOPERATIONS_ACK;
}

void TestCppClient::orderOperations()
{
	/*** Requesting the next valid id ***/
	//! [reqids]
	//The parameter is always ignored.
	m_pClient->reqIds(-1);
	//! [reqids]
	//! [reqallopenorders]
	m_pClient->reqAllOpenOrders();
	//! [reqallopenorders]
	//! [reqautoopenorders]
	m_pClient->reqAutoOpenOrders(true);
	//! [reqautoopenorders]
	//! [reqopenorders]
	m_pClient->reqOpenOrders();
	//! [reqopenorders]

	/*** Placing/modifying an order - remember to ALWAYS increment the nextValidId after placing an order so it can be used for the next one! ***/
    //! [order_submission]
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOrder("SELL", 1, 50));
    //! [order_submission]

	//m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::Block("BUY", 50, 20));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::BoxTop("SELL", 10));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::FutureComboContract(), OrderSamples::ComboLimitOrder("SELL", 1, 1, false));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::StockComboContract(), OrderSamples::ComboMarketOrder("BUY", 1, false));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::OptionComboContract(), OrderSamples::ComboMarketOrder("BUY", 1, true));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::StockComboContract(), OrderSamples::LimitOrderForComboWithLegPrices("BUY", 1, std::vector<double>(10, 5), true));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::Discretionary("SELL", 1, 45, 0.5));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtBox(), OrderSamples::LimitIfTouched("BUY", 1, 30, 34));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOnClose("SELL", 1, 34));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::LimitOnOpen("BUY", 1, 35));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketIfTouched("BUY", 1, 35));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOnClose("SELL", 1));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOnOpen("BUY", 1));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketOrder("SELL", 1));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::MarketToLimit("BUY", 1));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::OptionAtIse(), OrderSamples::MidpointMatch("BUY", 1));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::Stop("SELL", 1, 34.4));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::StopLimit("BUY", 1, 35, 33));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::StopWithProtection("SELL", 1, 45));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::SweepToFill("BUY", 1, 35));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::TrailingStop("SELL", 1, 0.5, 30));
	//m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), OrderSamples::TrailingStopLimit("BUY", 1, 2, 5, 50));

	//! [place order with cashQty]
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), OrderSamples::LimitOrderWithCashQty("BUY", 1, 30, 5000));
	//! [place order with cashQty]

	std::this_thread::sleep_for(std::chrono::seconds(1));

	/*** Cancel one order ***/
	//! [cancelorder]
	m_pClient->cancelOrder(m_orderId-1);
	//! [cancelorder]
	
	/*** Cancel all orders for all accounts ***/
	//! [reqglobalcancel]
	m_pClient->reqGlobalCancel();
	//! [reqglobalcancel]

	/*** Request the day's executions ***/
	//! [reqexecutions]
	m_pClient->reqExecutions(10001, ExecutionFilter());
	//! [reqexecutions]

	m_state = ST_ORDEROPERATIONS_ACK;
}

void TestCppClient::ocaSamples()
{
	//OCA ORDER
	//! [ocasubmit]
	std::vector<Order> ocaOrders;
	ocaOrders.push_back(OrderSamples::LimitOrder("BUY", 1, 10));
	ocaOrders.push_back(OrderSamples::LimitOrder("BUY", 1, 11));
	ocaOrders.push_back(OrderSamples::LimitOrder("BUY", 1, 12));
	for(unsigned int i = 0; i < ocaOrders.size(); i++){
		OrderSamples::OneCancelsAll("TestOca", ocaOrders[i], 2);
		m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), ocaOrders[i]);
	}
	//! [ocasubmit]

	m_state = ST_OCASAMPLES_ACK;
}

void TestCppClient::conditionSamples()
{
	//! [order_conditioning_activate]
	Order lmt = OrderSamples::LimitOrder("BUY", 100, 10);
	//Order will become active if conditioning criteria is met
	PriceCondition* priceCondition = dynamic_cast<PriceCondition *>(OrderSamples::Price_Condition(208813720, "SMART", 600, false, false));
	ExecutionCondition* execCondition = dynamic_cast<ExecutionCondition *>(OrderSamples::Execution_Condition("EUR.USD", "CASH", "IDEALPRO", true));
	MarginCondition* marginCondition = dynamic_cast<MarginCondition *>(OrderSamples::Margin_Condition(30, true, false));
	PercentChangeCondition* pctChangeCondition = dynamic_cast<PercentChangeCondition *>(OrderSamples::Percent_Change_Condition(15.0, 208813720, "SMART", true, true));
	TimeCondition* timeCondition = dynamic_cast<TimeCondition *>(OrderSamples::Time_Condition("20160118 23:59:59", true, false));
	VolumeCondition* volumeCondition = dynamic_cast<VolumeCondition *>(OrderSamples::Volume_Condition(208813720, "SMART", false, 100, true));

	lmt.conditions.push_back(ibapi::shared_ptr<PriceCondition>(priceCondition));
	lmt.conditions.push_back(ibapi::shared_ptr<ExecutionCondition>(execCondition));
	lmt.conditions.push_back(ibapi::shared_ptr<MarginCondition>(marginCondition));
	lmt.conditions.push_back(ibapi::shared_ptr<PercentChangeCondition>(pctChangeCondition));
	lmt.conditions.push_back(ibapi::shared_ptr<TimeCondition>(timeCondition));
	lmt.conditions.push_back(ibapi::shared_ptr<VolumeCondition>(volumeCondition));
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(),lmt);
	//! [order_conditioning_activate]

	//Conditions can make the order active or cancel it. Only LMT orders can be conditionally canceled.
	//! [order_conditioning_cancel]
	Order lmt2 = OrderSamples::LimitOrder("BUY", 100, 20);
	//The active order will be cancelled if conditioning criteria is met
	lmt2.conditionsCancelOrder = true;
	PriceCondition* priceCondition2 = dynamic_cast<PriceCondition *>(OrderSamples::Price_Condition(208813720, "SMART", 600, false, false));
	lmt2.conditions.push_back(ibapi::shared_ptr<PriceCondition>(priceCondition2));
	m_pClient->placeOrder(m_orderId++, ContractSamples::EuropeanStock(), lmt2);
	//! [order_conditioning_cancel]

	m_state = ST_CONDITIONSAMPLES_ACK;
}

void TestCppClient::bracketSample(){
	Order parent;
	Order takeProfit;
	Order stopLoss;
	//! [bracketsubmit]
	OrderSamples::BracketOrder(m_orderId++, parent, takeProfit, stopLoss, "BUY", 100, 30, 40, 20);
	m_pClient->placeOrder(parent.orderId, ContractSamples::EuropeanStock(), parent);
	m_pClient->placeOrder(takeProfit.orderId, ContractSamples::EuropeanStock(), takeProfit);
	m_pClient->placeOrder(stopLoss.orderId, ContractSamples::EuropeanStock(), stopLoss);
	//! [bracketsubmit]

	m_state = ST_BRACKETSAMPLES_ACK;
}

void TestCppClient::hedgeSample(){
	//F Hedge order
	//! [hedgesubmit]
	//Parent order on a contract which currency differs from your base currency
	Order parent = OrderSamples::LimitOrder("BUY", 100, 10);
	parent.orderId = m_orderId++;
	//Hedge on the currency conversion
	Order hedge = OrderSamples::MarketFHedge(parent.orderId, "BUY");
	//Place the parent first...
	m_pClient->placeOrder(parent.orderId, ContractSamples::EuropeanStock(), parent);
	//Then the hedge order
	m_pClient->placeOrder(m_orderId++, ContractSamples::EurGbpFx(), hedge);
	//! [hedgesubmit]

	m_state = ST_HEDGESAMPLES_ACK;
}

void TestCppClient::testAlgoSamples(){
	//! [algo_base_order]
	Order baseOrder = OrderSamples::LimitOrder("BUY", 1000, 1);
	//! [algo_base_order]

	//! [arrivalpx]
	AvailableAlgoParams::FillArrivalPriceParams(baseOrder, 0.1, "Aggressive", "09:00:00 CET", "16:00:00 CET", true, true, 100000);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [arrivalpx]

	//! [darkice]
	AvailableAlgoParams::FillDarkIceParams(baseOrder, 10, "09:00:00 CET", "16:00:00 CET", true, 100000);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [darkice]

	//! [ad]
	// The Time Zone in "startTime" and "endTime" attributes is ignored and always defaulted to GMT
	AvailableAlgoParams::FillAccumulateDistributeParams(baseOrder, 10, 60, true, true, 1, true, true, "20161010-12:00:00 GMT", "20161010-16:00:00 GMT");
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [ad]

	//! [twap]
	AvailableAlgoParams::FillTwapParams(baseOrder, "Marketable", "09:00:00 CET", "16:00:00 CET", true, 100000);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [twap]

	//! [vwap]
	AvailableAlgoParams::FillBalanceImpactRiskParams(baseOrder, 0.1, "Aggressive", true);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	 //! [vwap]

	//! [balanceimpactrisk]
	AvailableAlgoParams::FillBalanceImpactRiskParams(baseOrder, 0.1, "Aggressive", true);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [balanceimpactrisk]

	//! [minimpact]
	AvailableAlgoParams::FillMinImpactParams(baseOrder, 0.3);
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [minimpact]

	//! [adaptive]
	AvailableAlgoParams::FillAdaptiveParams(baseOrder, "Normal");
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
	//! [adaptive]

	//! [closepx]
    AvailableAlgoParams::FillClosePriceParams(baseOrder, 0.5, "Neutral", "12:00:00 EST", true, 100000);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
    //! [closepx]
    
    //! [pctvol]
    AvailableAlgoParams::FillPctVolParams(baseOrder, 0.5, "12:00:00 EST", "14:00:00 EST", true, 100000);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
    //! [pctvol]               
    
    //! [pctvolpx]
    AvailableAlgoParams::FillPriceVariantPctVolParams(baseOrder, 0.1, 0.05, 0.01, 0.2, "12:00:00 EST", "14:00:00 EST", true, 100000);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
    //! [pctvolpx]
    
    //! [pctvolsz]
    AvailableAlgoParams::FillSizeVariantPctVolParams(baseOrder, 0.2, 0.4, "12:00:00 EST", "14:00:00 EST", true, 100000);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
    //! [pctvolsz]
    
    //! [pctvoltm]
    AvailableAlgoParams::FillTimeVariantPctVolParams(baseOrder, 0.2, 0.4, "12:00:00 EST", "14:00:00 EST", true, 100000);
    m_pClient->placeOrder(m_orderId++, ContractSamples::USStockAtSmart(), baseOrder);
    //! [pctvoltm]

	//! [jeff_vwap_algo]
	AvailableAlgoParams::FillJefferiesVWAPParams(baseOrder, "10:00:00 EST", "16:00:00 EST", 10, 10, "Exclude_Both", 130, 135, 1, 10, "Patience", false, "Midpoint");
	m_pClient->placeOrder(m_orderId++, ContractSamples::JefferiesContract(), baseOrder);
	//! [jeff_vwap_algo]

	//! [csfb_inline_algo]
	AvailableAlgoParams::FillCSFBInlineParams(baseOrder, "10:00:00 EST", "16:00:00 EST", "Patient", 10, 20, 100, "Default", false, 40, 100, 100, 35);
	m_pClient->placeOrder(m_orderId++, ContractSamples::CSFBContract(), baseOrder);
	//! [csfb_inline_algo]
	
	m_state = ST_TESTALGOSAMPLES_ACK;
}

void TestCppClient::financialAdvisorOrderSamples()
{
	//! [faorderoneaccount]
	Order faOrderOneAccount = OrderSamples::MarketOrder("BUY", 100);
	// Specify the Account Number directly
	faOrderOneAccount.account = "DU119915";
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), faOrderOneAccount);
	//! [faorderoneaccount]
	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [faordergroupequalquantity]
	Order faOrderGroupEQ = OrderSamples::LimitOrder("SELL", 200, 2000);
	faOrderGroupEQ.faGroup = "Group_Equal_Quantity";
	faOrderGroupEQ.faMethod = "EqualQuantity";
	m_pClient->placeOrder(m_orderId++, ContractSamples::SimpleFuture(), faOrderGroupEQ);
	//! [faordergroupequalquantity]
	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [faordergrouppctchange]
	Order faOrderGroupPC;
	faOrderGroupPC.action = "BUY";
	faOrderGroupPC.orderType = "MKT";
	// You should not specify any order quantity for PctChange allocation method
	faOrderGroupPC.faGroup = "Pct_Change";
	faOrderGroupPC.faMethod = "PctChange";
	faOrderGroupPC.faPercentage = "100";
	m_pClient->placeOrder(m_orderId++, ContractSamples::EurGbpFx(), faOrderGroupPC);
	//! [faordergrouppctchange]
	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [faorderprofile]
	Order faOrderProfile = OrderSamples::LimitOrder("BUY", 200, 100);
	faOrderProfile.faProfile = "Percent_60_40";
	m_pClient->placeOrder(m_orderId++, ContractSamples::EuropeanStock(), faOrderProfile);
	//! [faorderprofile]

	//! [modelorder]
	Order modelOrder = OrderSamples::LimitOrder("BUY", 200, 100);
	modelOrder.account = "DF12345";
	modelOrder.modelCode = "Technology";
	m_pClient->placeOrder(m_orderId++, ContractSamples::USStock(), modelOrder);
	//! [modelorder]

	m_state = ST_FAORDERSAMPLES_ACK;
}

void TestCppClient::financialAdvisorOperations()
{
	/*** Requesting FA information ***/
	//! [requestfaaliases]
	m_pClient->requestFA(faDataType::ALIASES);
	//! [requestfaaliases]

	//! [requestfagroups]
	m_pClient->requestFA(faDataType::GROUPS);
	//! [requestfagroups]

	//! [requestfaprofiles]
	m_pClient->requestFA(faDataType::PROFILES);
	//! [requestfaprofiles]

	/*** Replacing FA information - Fill in with the appropriate XML string. ***/
	//! [replacefaonegroup]
	m_pClient->replaceFA(faDataType::GROUPS, FAMethodSamples::FAOneGroup());
	//! [replacefaonegroup]

	//! [replacefatwogroups]
	m_pClient->replaceFA(faDataType::GROUPS, FAMethodSamples::FATwoGroups());
	//! [replacefatwogroups]

	//! [replacefaoneprofile]
	m_pClient->replaceFA(faDataType::PROFILES, FAMethodSamples::FAOneProfile());
	//! [replacefaoneprofile]

	//! [replacefatwoprofiles]
	m_pClient->replaceFA(faDataType::PROFILES, FAMethodSamples::FATwoProfiles());
	//! [replacefatwoprofiles]

	//! [reqSoftDollarTiers]
	m_pClient->reqSoftDollarTiers(4001);
	//! [reqSoftDollarTiers]

	m_state = ST_FAOPERATIONS_ACK;
}

void TestCppClient::testDisplayGroups(){
	//! [querydisplaygroups]
	m_pClient->queryDisplayGroups(9001);
	//! [querydisplaygroups]

	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [subscribetogroupevents]
	m_pClient->subscribeToGroupEvents(9002, 1);
	//! [subscribetogroupevents]

	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [updatedisplaygroup]
	m_pClient->updateDisplayGroup(9002, "8314@SMART");
	//! [updatedisplaygroup]

	std::this_thread::sleep_for(std::chrono::seconds(1));

	//! [subscribefromgroupevents]
	m_pClient->unsubscribeFromGroupEvents(9002);
	//! [subscribefromgroupevents]

	m_state = ST_TICKDATAOPERATION_ACK;
}

void TestCppClient::miscelaneous()
{
	/*** Request TWS' current time ***/
	m_pClient->reqCurrentTime();
	/*** Setting TWS logging level  ***/
	m_pClient->setServerLogLevel(5);

	m_state = ST_MISCELANEOUS_ACK;
}

void TestCppClient::reqFamilyCodes()
{
	/*** Request TWS' family codes ***/
	//! [reqfamilycodes]
	m_pClient->reqFamilyCodes();
	//! [reqfamilycodes]

	m_state = ST_FAMILYCODES_ACK;
}

void TestCppClient::reqMatchingSymbols()
{
	/*** Request TWS' mathing symbols ***/
	//! [reqmatchingsymbols]
	m_pClient->reqMatchingSymbols(11001, "IBM");
	//! [reqmatchingsymbols]
	m_state = ST_SYMBOLSAMPLES_ACK;
}

void TestCppClient::reqMktDepthExchanges()
{
	/*** Request TWS' market depth exchanges ***/
	//! [reqMktDepthExchanges]
	m_pClient->reqMktDepthExchanges();
	//! [reqMktDepthExchanges]

	m_state = ST_REQMKTDEPTHEXCHANGES_ACK;
}

void TestCppClient::reqNewsTicks()
{
	//! [reqmktdata_ticknews]
	m_pClient->reqMktData(12001, ContractSamples::USStockAtSmart(), "mdoff,292", false, false, TagValueListSPtr());
	//! [reqmktdata_ticknews]

	std::this_thread::sleep_for(std::chrono::seconds(5));

	//! [cancelmktdata2]
	m_pClient->cancelMktData(12001);
	//! [cancelmktdata2]

	m_state = ST_REQNEWSTICKS_ACK;
}

void TestCppClient::reqSmartComponents()
{
	static bool bFirstRun = true;

	if (bFirstRun) {
		m_pClient->reqMktData(13001, ContractSamples::USStockAtSmart(), "", false, false, TagValueListSPtr());

		bFirstRun = false;
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));

	if (m_bboExchange.size() > 0) {
		m_pClient->cancelMktData(13001);

		//! [reqsmartcomponents]
		m_pClient->reqSmartComponents(13002, m_bboExchange);
		//! [reqsmartcomponents]
		m_state = ST_REQSMARTCOMPONENTS_ACK;
	}
}

void TestCppClient::reqNewsProviders()
{
	/*** Request TWS' news providers ***/
	//! [reqNewsProviders]
	m_pClient->reqNewsProviders();
	//! [reqNewsProviders]

	m_state = ST_NEWSPROVIDERS_ACK;
}

void TestCppClient::reqNewsArticle()
{
	/*** Request TWS' news article ***/
	//! [reqNewsArticle]
	TagValueList* list = new TagValueList();
	list->push_back((TagValueSPtr)new TagValue("manual", "1"));
	m_pClient->reqNewsArticle(12001, "BZ", "BZ$04507322", TagValueListSPtr(list));
	//! [reqNewsArticle]

	m_state = ST_REQNEWSARTICLE_ACK;
}

void TestCppClient::reqHistoricalNews(){

	//! [reqHistoricalNews]
	TagValueList* list = new TagValueList();
	list->push_back((TagValueSPtr)new TagValue("manual", "1"));
	m_pClient->reqHistoricalNews(12001, 8314, "BZ+FLY", "", "", 5, TagValueListSPtr(list));
	//! [reqHistoricalNews]

	std::this_thread::sleep_for(std::chrono::seconds(1));

	m_state = ST_REQHISTORICALNEWS_ACK;
}


void TestCppClient::reqHeadTimestamp() {
	//! [reqHeadTimeStamp]
	m_pClient->reqHeadTimestamp(14001, ContractSamples::EurGbpFx(), "MIDPOINT", 1, 1);
	//! [reqHeadTimeStamp]	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//! [cancelHeadTimestamp]
	m_pClient->cancelHeadTimestamp(14001);
	//! [cancelHeadTimestamp]
	
	m_state = ST_REQHEADTIMESTAMP_ACK;
}

void TestCppClient::reqHistogramData() {
	//! [reqHistogramData]
	m_pClient->reqHistogramData(15001, ContractSamples::IBMUSStockAtSmart(), false, "1 weeks");
	//! [reqHistogramData]
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//! [cancelHistogramData]
	m_pClient->cancelHistogramData(15001);
	//! [cancelHistogramData]
	m_state = ST_REQHISTOGRAMDATA_ACK;
}

void TestCppClient::rerouteCFDOperations()
{
    //! [reqmktdatacfd]
	m_pClient->reqMktData(16001, ContractSamples::USStockCFD(), "", false, false, TagValueListSPtr());
    std::this_thread::sleep_for(std::chrono::seconds(1));
	m_pClient->reqMktData(16002, ContractSamples::EuropeanStockCFD(), "", false, false, TagValueListSPtr());
    std::this_thread::sleep_for(std::chrono::seconds(1));
	m_pClient->reqMktData(16003, ContractSamples::CashCFD(), "", false, false, TagValueListSPtr());
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//! [reqmktdatacfd]

    //! [reqmktdepthcfd]
	m_pClient->reqMktDepth(16004, ContractSamples::USStockCFD(), 10, TagValueListSPtr());
    std::this_thread::sleep_for(std::chrono::seconds(1));
	m_pClient->reqMktDepth(16005, ContractSamples::EuropeanStockCFD(), 10, TagValueListSPtr());
    std::this_thread::sleep_for(std::chrono::seconds(1));
	m_pClient->reqMktDepth(16006, ContractSamples::CashCFD(), 10, TagValueListSPtr());
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//! [reqmktdepthcfd]

	m_state = ST_REROUTECFD_ACK;
}

void TestCppClient::marketRuleOperations()
{
	m_pClient->reqContractDetails(17001, ContractSamples::IBMBond());
	m_pClient->reqContractDetails(17002, ContractSamples::IBKRStk());

    std::this_thread::sleep_for(std::chrono::seconds(2));

	//! [reqmarketrule]
	m_pClient->reqMarketRule(26);
	m_pClient->reqMarketRule(635);
	m_pClient->reqMarketRule(1388);
	//! [reqmarketrule]

	m_state = ST_MARKETRULE_ACK;
}

void TestCppClient::continuousFuturesOperations()
{
	m_pClient->reqContractDetails(18001, ContractSamples::ContFut());

	//! [reqhistoricaldatacontfut]
	std::time_t rawtime;
    std::tm* timeinfo;
    char queryTime [80];

	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
	std::strftime(queryTime, 80, "%Y%m%d %H:%M:%S", timeinfo);

	m_pClient->reqHistoricalData(18002, ContractSamples::ContFut(), queryTime, "1 Y", "1 month", "TRADES", 0, 1, false, TagValueListSPtr());

    std::this_thread::sleep_for(std::chrono::seconds(10));

	m_pClient->cancelHistoricalData(18002);
	//! [reqhistoricaldatacontfut]

	m_state = ST_CONTFUT_ACK;
}

void TestCppClient::reqHistoricalTicks() 
{
	//! [reqhistoricalticks]
    m_pClient->reqHistoricalTicks(19001, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33", "", 10, "BID_ASK", 1, true, TagValueListSPtr());
    m_pClient->reqHistoricalTicks(19002, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33", "", 10, "MIDPOINT", 1, true, TagValueListSPtr());
    m_pClient->reqHistoricalTicks(19003, ContractSamples::IBMUSStockAtSmart(), "20170621 09:38:33", "", 10, "TRADES", 1, true, TagValueListSPtr());
    //! [reqhistoricalticks]
    m_state = ST_REQHISTORICALTICKS_ACK;
}

void TestCppClient::reqTickByTickData() 
{
    /*** Requesting tick-by-tick data (only refresh) ***/
    //! [reqtickbytickdata]
    m_pClient->reqTickByTickData(20001, ContractSamples::IBMUSStockAtSmart(), "Last");
    m_pClient->reqTickByTickData(20002, ContractSamples::IBMUSStockAtSmart(), "AllLast");
    m_pClient->reqTickByTickData(20003, ContractSamples::IBMUSStockAtSmart(), "BidAsk");
    m_pClient->reqTickByTickData(20004, ContractSamples::EurGbpFx(), "MidPoint");

    std::this_thread::sleep_for(std::chrono::seconds(10));

    m_pClient->cancelTickByTickData(20001);
    m_pClient->cancelTickByTickData(20002);
    m_pClient->cancelTickByTickData(20003);
    m_pClient->cancelTickByTickData(20004);
    //! [reqtickbytickdata]

    m_state = ST_REQTICKBYTICKDATA_ACK;
}

//! [nextvalidid]
void TestCppClient::nextValidId( OrderId orderId)
{
	printf("Next Valid Id: %ld\n", orderId);
	m_orderId = orderId;
	//! [nextvalidid]

    m_state = ST_REQTICKBYTICKDATA; 
    //m_state = ST_REQHISTORICALTICKS; 
    //m_state = ST_CONTFUT; 
    //m_state = ST_PNLSINGLE; 
    //m_state = ST_PNL; 
	//m_state = ST_DELAYEDTICKDATAOPERATION; 
	//m_state = ST_MARKETDEPTHOPERATION;
	//m_state = ST_REALTIMEBARS;
	//m_state = ST_MARKETDATATYPE;
	//m_state = ST_HISTORICALDATAREQUESTS;
	//m_state = ST_CONTRACTOPERATION;
	//m_state = ST_MARKETSCANNERS;
	//m_state = ST_REUTERSFUNDAMENTALS;
	//m_state = ST_BULLETINS;
	//m_state = ST_ACCOUNTOPERATIONS;
	//m_state = ST_ORDEROPERATIONS;
	//m_state = ST_OCASAMPLES;
	//m_state = ST_CONDITIONSAMPLES;
	//m_state = ST_BRACKETSAMPLES;
	//m_state = ST_HEDGESAMPLES;
	//m_state = ST_TESTALGOSAMPLES;
	//m_state = ST_FAORDERSAMPLES;
	//m_state = ST_FAOPERATIONS;
	//m_state = ST_DISPLAYGROUPS;
	//m_state = ST_MISCELANEOUS;
	//m_state = ST_FAMILYCODES;
	//m_state = ST_SYMBOLSAMPLES;
	//m_state = ST_REQMKTDEPTHEXCHANGES;
	//m_state = ST_REQNEWSTICKS;
	//m_state = ST_REQSMARTCOMPONENTS;
	//m_state = ST_NEWSPROVIDERS;
	//m_state = ST_REQNEWSARTICLE;
	//m_state = ST_REQHISTORICALNEWS;
	//m_state = ST_REQHEADTIMESTAMP;
	//m_state = ST_REQHISTOGRAMDATA;
	//m_state = ST_REROUTECFD;
	//m_state = ST_MARKETRULE;
	//m_state = ST_PING;
}


void TestCppClient::currentTime( long time)
{
	if ( m_state == ST_PING_ACK) {
		time_t t = ( time_t)time;
		struct tm * timeinfo = localtime ( &t);
		printf( "The current date/time is: %s", asctime( timeinfo));

		time_t now = ::time(NULL);
		m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;

		m_state = ST_PING_ACK;
	}
}

//! [error]
void TestCppClient::error(const int id, const int errorCode, const std::string errorString)
{
	printf( "Error. Id: %d, Code: %d, Msg: %s\n", id, errorCode, errorString.c_str());
}
//! [error]

//! [tickprice]
void TestCppClient::tickPrice( TickerId tickerId, TickType field, double price, const TickAttrib& attribs) {
	printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", tickerId, (int)field, price, attribs.canAutoExecute, attribs.pastLimit, attribs.preOpen);
}
//! [tickprice]

//! [ticksize]
void TestCppClient::tickSize( TickerId tickerId, TickType field, int size) {
	printf( "Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", tickerId, (int)field, size);
}
//! [ticksize]

//! [tickoptioncomputation]
void TestCppClient::tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                          double optPrice, double pvDividend,
                                          double gamma, double vega, double theta, double undPrice) {
	printf( "TickOptionComputation. Ticker Id: %ld, Type: %d, ImpliedVolatility: %g, Delta: %g, OptionPrice: %g, pvDividend: %g, Gamma: %g, Vega: %g, Theta: %g, Underlying Price: %g\n", tickerId, (int)tickType, impliedVol, delta, optPrice, pvDividend, gamma, vega, theta, undPrice);
}
//! [tickoptioncomputation]

//! [tickgeneric]
void TestCppClient::tickGeneric(TickerId tickerId, TickType tickType, double value) {
	printf( "Tick Generic. Ticker Id: %ld, Type: %d, Value: %g\n", tickerId, (int)tickType, value);
}
//! [tickgeneric]

//! [tickstring]
void TestCppClient::tickString(TickerId tickerId, TickType tickType, const std::string& value) {
	printf( "Tick String. Ticker Id: %ld, Type: %d, Value: %s\n", tickerId, (int)tickType, value.c_str());
}
//! [tickstring]

void TestCppClient::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
                            double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {
	printf( "TickEFP. %ld, Type: %d, BasisPoints: %g, FormattedBasisPoints: %s, Total Dividends: %g, HoldDays: %d, Future Last Trade Date: %s, Dividend Impact: %g, Dividends To Last Trade Date: %g\n", tickerId, (int)tickType, basisPoints, formattedBasisPoints.c_str(), totalDividends, holdDays, futureLastTradeDate.c_str(), dividendImpact, dividendsToLastTradeDate);
}

//! [orderstatus]
void TestCppClient::orderStatus(OrderId orderId, const std::string& status, double filled,
		double remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice){
	printf("OrderStatus. Id: %ld, Status: %s, Filled: %g, Remaining: %g, AvgFillPrice: %g, PermId: %d, LastFillPrice: %g, ClientId: %d, WhyHeld: %s, MktCapPrice: %g, lastLiquidity: %d\n", orderId, status.c_str(), filled, remaining, avgFillPrice, permId, lastFillPrice, clientId, whyHeld.c_str(), mktCapPrice);
}
//! [orderstatus]

//! [openorder]
void TestCppClient::openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& ostate) {
	printf( "OpenOrder. ID: %ld, %s, %s @ %s: %s, %s, %g, %g, %s\n", orderId, contract.symbol.c_str(), contract.secType.c_str(), contract.exchange.c_str(), order.action.c_str(), order.orderType.c_str(), order.totalQuantity, order.cashQty == UNSET_DOUBLE ? 0 : order.cashQty, ostate.status.c_str());
}
//! [openorder]

//! [openorderend]
void TestCppClient::openOrderEnd() {
	printf( "OpenOrderEnd\n");
}
//! [openorderend]

void TestCppClient::winError( const std::string& str, int lastError) {}
void TestCppClient::connectionClosed() {
	printf( "Connection Closed\n");
}

//! [updateaccountvalue]
void TestCppClient::updateAccountValue(const std::string& key, const std::string& val,
                                       const std::string& currency, const std::string& accountName) {
	printf("UpdateAccountValue. Key: %s, Value: %s, Currency: %s, Account Name: %s\n", key.c_str(), val.c_str(), currency.c_str(), accountName.c_str());
}
//! [updateaccountvalue]

//! [updateportfolio]
void TestCppClient::updatePortfolio(const Contract& contract, double position,
                                    double marketPrice, double marketValue, double averageCost,
                                    double unrealizedPNL, double realizedPNL, const std::string& accountName){
	printf("UpdatePortfolio. %s, %s @ %s: Position: %g, MarketPrice: %g, MarketValue: %g, AverageCost: %g, UnrealizedPNL: %g, RealizedPNL: %g, AccountName: %s\n", (contract.symbol).c_str(), (contract.secType).c_str(), (contract.primaryExchange).c_str(), position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, accountName.c_str());
}
//! [updateportfolio]

//! [updateaccounttime]
void TestCppClient::updateAccountTime(const std::string& timeStamp) {
	printf( "UpdateAccountTime. Time: %s\n", timeStamp.c_str());
}
//! [updateaccounttime]

//! [accountdownloadend]
void TestCppClient::accountDownloadEnd(const std::string& accountName) {
	printf( "Account download finished: %s\n", accountName.c_str());
}
//! [accountdownloadend]

//! [contractdetails]
void TestCppClient::contractDetails( int reqId, const ContractDetails& contractDetails) {
	printf( "ContractDetails begin. ReqId: %d\n", reqId);
	printContractMsg(contractDetails.summary);
	printContractDetailsMsg(contractDetails);
	printf( "ContractDetails end. ReqId: %d\n", reqId);
}
//! [contractdetails]

//! [bondcontractdetails]
void TestCppClient::bondContractDetails( int reqId, const ContractDetails& contractDetails) {
	printf( "BondContractDetails begin. ReqId: %d\n", reqId);
	printBondContractDetailsMsg(contractDetails);
	printf( "BondContractDetails end. ReqId: %d\n", reqId);
}
//! [bondcontractdetails]

void TestCppClient::printContractMsg(const Contract& contract) {
	printf("\tConId: %d\n", contract.conId);
	printf("\tSymbol: %s\n", contract.symbol.c_str());
	printf("\tSecType: %s\n", contract.secType.c_str());
	printf("\tLastTradeDateOrContractMonth: %s\n", contract.lastTradeDateOrContractMonth.c_str());
	printf("\tStrike: %g\n", contract.strike);
	printf("\tRight: %s\n", contract.right.c_str());
	printf("\tMultiplier: %s\n", contract.multiplier.c_str());
	printf("\tExchange: %s\n", contract.exchange.c_str());
	printf("\tPrimaryExchange: %s\n", contract.primaryExchange.c_str());
	printf("\tCurrency: %s\n", contract.currency.c_str());
	printf("\tLocalSymbol: %s\n", contract.localSymbol.c_str());
	printf("\tTradingClass: %s\n", contract.tradingClass.c_str());
}

void TestCppClient::printContractDetailsMsg(const ContractDetails& contractDetails) {
	printf("\tMarketName: %s\n", contractDetails.marketName.c_str());
	printf("\tMinTick: %g\n", contractDetails.minTick);
	printf("\tPriceMagnifier: %d\n", contractDetails.priceMagnifier);
	printf("\tOrderTypes: %s\n", contractDetails.orderTypes.c_str());
	printf("\tValidExchanges: %s\n", contractDetails.validExchanges.c_str());
	printf("\tUnderConId: %d\n", contractDetails.underConId);
	printf("\tLongName: %s\n", contractDetails.longName.c_str());
	printf("\tContractMonth: %s\n", contractDetails.contractMonth.c_str());
	printf("\tIndystry: %s\n", contractDetails.industry.c_str());
	printf("\tCategory: %s\n", contractDetails.category.c_str());
	printf("\tSubCategory: %s\n", contractDetails.subcategory.c_str());
	printf("\tTimeZoneId: %s\n", contractDetails.timeZoneId.c_str());
	printf("\tTradingHours: %s\n", contractDetails.tradingHours.c_str());
	printf("\tLiquidHours: %s\n", contractDetails.liquidHours.c_str());
	printf("\tEvRule: %s\n", contractDetails.evRule.c_str());
	printf("\tEvMultiplier: %g\n", contractDetails.evMultiplier);
	printf("\tMdSizeMultiplier: %d\n", contractDetails.mdSizeMultiplier);
	printf("\tAggGroup: %d\n", contractDetails.aggGroup);
	printf("\tUnderSymbol: %s\n", contractDetails.underSymbol.c_str());
	printf("\tUnderSecType: %s\n", contractDetails.underSecType.c_str());
	printf("\tMarketRuleIds: %s\n", contractDetails.marketRuleIds.c_str());
	printf("\tRealExpirationDate: %s\n", contractDetails.realExpirationDate.c_str());
	printContractDetailsSecIdList(contractDetails.secIdList);
}

void TestCppClient::printContractDetailsSecIdList(const TagValueListSPtr &secIdList) {
	const int secIdListCount = secIdList.get() ? secIdList->size() : 0;
	if (secIdListCount > 0) {
		printf("\tSecIdList: {");
		for (int i = 0; i < secIdListCount; ++i) {
			const TagValue* tagValue = ((*secIdList)[i]).get();
			printf("%s=%s;",tagValue->tag.c_str(), tagValue->value.c_str());
		}
		printf("}\n");
	}
}

void TestCppClient::printBondContractDetailsMsg(const ContractDetails& contractDetails) {
	printf("\tSymbol: %s\n", contractDetails.summary.symbol.c_str());
	printf("\tSecType: %s\n", contractDetails.summary.secType.c_str());
	printf("\tCusip: %s\n", contractDetails.cusip.c_str());
	printf("\tCoupon: %g\n", contractDetails.coupon);
	printf("\tMaturity: %s\n", contractDetails.maturity.c_str());
	printf("\tIssueDate: %s\n", contractDetails.issueDate.c_str());
	printf("\tRatings: %s\n", contractDetails.ratings.c_str());
	printf("\tBondType: %s\n", contractDetails.bondType.c_str());
	printf("\tCouponType: %s\n", contractDetails.couponType.c_str());
	printf("\tConvertible: %s\n", contractDetails.convertible ? "yes" : "no");
	printf("\tCallable: %s\n", contractDetails.callable ? "yes" : "no");
	printf("\tPutable: %s\n", contractDetails.putable ? "yes" : "no");
	printf("\tDescAppend: %s\n", contractDetails.descAppend.c_str());
	printf("\tExchange: %s\n", contractDetails.summary.exchange.c_str());
	printf("\tCurrency: %s\n", contractDetails.summary.currency.c_str());
	printf("\tMarketName: %s\n", contractDetails.marketName.c_str());
	printf("\tTradingClass: %s\n", contractDetails.summary.tradingClass.c_str());
	printf("\tConId: %d\n", contractDetails.summary.conId);
	printf("\tMinTick: %g\n", contractDetails.minTick);
	printf("\tMdSizeMultiplier: %d\n", contractDetails.mdSizeMultiplier);
	printf("\tOrderTypes: %s\n", contractDetails.orderTypes.c_str());
	printf("\tValidExchanges: %s\n", contractDetails.validExchanges.c_str());
	printf("\tNextOptionDate: %s\n", contractDetails.nextOptionDate.c_str());
	printf("\tNextOptionType: %s\n", contractDetails.nextOptionType.c_str());
	printf("\tNextOptionPartial: %s\n", contractDetails.nextOptionPartial ? "yes" : "no");
	printf("\tNotes: %s\n", contractDetails.notes.c_str());
	printf("\tLong Name: %s\n", contractDetails.longName.c_str());
	printf("\tEvRule: %s\n", contractDetails.evRule.c_str());
	printf("\tEvMultiplier: %g\n", contractDetails.evMultiplier);
	printf("\tAggGroup: %d\n", contractDetails.aggGroup);
	printf("\tMarketRuleIds: %s\n", contractDetails.marketRuleIds.c_str());
	printContractDetailsSecIdList(contractDetails.secIdList);
}

//! [contractdetailsend]
void TestCppClient::contractDetailsEnd( int reqId) {
	printf( "ContractDetailsEnd. %d\n", reqId);
}
//! [contractdetailsend]

//! [execdetails]
void TestCppClient::execDetails( int reqId, const Contract& contract, const Execution& execution) {
	printf( "ExecDetails. ReqId: %d - %s, %s, %s - %s, %ld, %g, %d\n", reqId, contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), execution.execId.c_str(), execution.orderId, execution.shares, execution.lastLiquidity);
}
//! [execdetails]

//! [execdetailsend]
void TestCppClient::execDetailsEnd( int reqId) {
	printf( "ExecDetailsEnd. %d\n", reqId);
}
//! [execdetailsend]

//! [updatemktdepth]
void TestCppClient::updateMktDepth(TickerId id, int position, int operation, int side,
                                   double price, int size) {
	printf( "UpdateMarketDepth. %ld - Position: %d, Operation: %d, Side: %d, Price: %g, Size: %d\n", id, position, operation, side, price, size);
}
//! [updatemktdepth]

//! [updatemktdepthl2]
void TestCppClient::updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                                     int side, double price, int size) {
	printf( "UpdateMarketDepthL2. %ld - Position: %d, Operation: %d, Side: %d, Price: %g, Size: %d\n", id, position, operation, side, price, size);
}
//! [updatemktdepthl2]

//! [updatenewsbulletin]
void TestCppClient::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) {
	printf( "News Bulletins. %d - Type: %d, Message: %s, Exchange of Origin: %s\n", msgId, msgType, newsMessage.c_str(), originExch.c_str());
}
//! [updatenewsbulletin]

//! [managedaccounts]
void TestCppClient::managedAccounts( const std::string& accountsList) {
	printf( "Account List: %s\n", accountsList.c_str());
}
//! [managedaccounts]

//! [receivefa]
void TestCppClient::receiveFA(faDataType pFaDataType, const std::string& cxml) {
	std::cout << "Receiving FA: " << (int)pFaDataType << std::endl << cxml << std::endl;
}
//! [receivefa]

//! [historicaldata]
void TestCppClient::historicalData(TickerId reqId, Bar bar) {
	printf( "HistoricalData. ReqId: %ld - Date: %s, Open: %g, High: %g, Low: %g, Close: %g, Volume: %d, Count: %d, WAP: %g\n", reqId, bar.time.c_str(), bar.open, bar.high, bar.low, bar.close, bar.volume, bar.count, bar.wap);
}
//! [historicaldata]

//! [historicaldataend]
void TestCppClient::historicalDataEnd(int reqId, std::string startDateStr, std::string endDateStr) { 
	std::cout << "HistoricalDataEnd. ReqId: " << reqId << " - Start Date: " << startDateStr << ", End Date: " << endDateStr << std::endl;	
}
//! [historicaldataend]

//! [scannerparameters]
void TestCppClient::scannerParameters(const std::string& xml) {
	printf( "ScannerParameters. %s\n", xml.c_str());
}
//! [scannerparameters]

//! [scannerdata]
void TestCppClient::scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                                const std::string& distance, const std::string& benchmark, const std::string& projection,
                                const std::string& legsStr) {
	printf( "ScannerData. %d - Rank: %d, Symbol: %s, SecType: %s, Currency: %s, Distance: %s, Benchmark: %s, Projection: %s, Legs String: %s\n", reqId, rank, contractDetails.summary.symbol.c_str(), contractDetails.summary.secType.c_str(), contractDetails.summary.currency.c_str(), distance.c_str(), benchmark.c_str(), projection.c_str(), legsStr.c_str());
}
//! [scannerdata]

//! [scannerdataend]
void TestCppClient::scannerDataEnd(int reqId) {
	printf( "ScannerDataEnd. %d\n", reqId);
}
//! [scannerdataend]

//! [realtimebar]
void TestCppClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                                long volume, double wap, int count) {
	printf( "RealTimeBars. %ld - Time: %ld, Open: %g, High: %g, Low: %g, Close: %g, Volume: %ld, Count: %d, WAP: %g\n", reqId, time, open, high, low, close, volume, count, wap);
}
//! [realtimebar]

//! [fundamentaldata]
void TestCppClient::fundamentalData(TickerId reqId, const std::string& data) {
	printf( "FundamentalData. ReqId: %ld, %s\n", reqId, data.c_str());
}
//! [fundamentaldata]

void TestCppClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {
	printf( "DeltaNeutralValidation. %d, ConId: %ld, Delta: %g, Price: %g\n", reqId, underComp.conId, underComp.delta, underComp.price);
}

//! [ticksnapshotend]
void TestCppClient::tickSnapshotEnd(int reqId) {
	printf( "TickSnapshotEnd: %d\n", reqId);
}
//! [ticksnapshotend]

//! [marketdatatype]
void TestCppClient::marketDataType(TickerId reqId, int marketDataType) {
	printf( "MarketDataType. ReqId: %ld, Type: %d\n", reqId, marketDataType);
}
//! [marketdatatype]

//! [commissionreport]
void TestCppClient::commissionReport( const CommissionReport& commissionReport) {
	printf( "CommissionReport. %s - %g %s RPNL %g\n", commissionReport.execId.c_str(), commissionReport.commission, commissionReport.currency.c_str(), commissionReport.realizedPNL);
}
//! [commissionreport]

//! [position]
void TestCppClient::position( const std::string& account, const Contract& contract, double position, double avgCost) {
	printf( "Position. %s - Symbol: %s, SecType: %s, Currency: %s, Position: %g, Avg Cost: %g\n", account.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), position, avgCost);
}
//! [position]

//! [positionend]
void TestCppClient::positionEnd() {
	printf( "PositionEnd\n");
}
//! [positionend]

//! [accountsummary]
void TestCppClient::accountSummary( int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& currency) {
	printf( "Acct Summary. ReqId: %d, Account: %s, Tag: %s, Value: %s, Currency: %s\n", reqId, account.c_str(), tag.c_str(), value.c_str(), currency.c_str());
}
//! [accountsummary]

//! [accountsummaryend]
void TestCppClient::accountSummaryEnd( int reqId) {
	printf( "AccountSummaryEnd. Req Id: %d\n", reqId);
}
//! [accountsummaryend]

void TestCppClient::verifyMessageAPI( const std::string& apiData) {
	printf("verifyMessageAPI: %s\b", apiData.c_str());
}

void TestCppClient::verifyCompleted( bool isSuccessful, const std::string& errorText) {
	printf("verifyCompleted. IsSuccessfule: %d - Error: %s\n", isSuccessful, errorText.c_str());
}

void TestCppClient::verifyAndAuthMessageAPI( const std::string& apiDatai, const std::string& xyzChallenge) {
	printf("verifyAndAuthMessageAPI: %s %s\n", apiDatai.c_str(), xyzChallenge.c_str());
}

void TestCppClient::verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) {
	printf("verifyAndAuthCompleted. IsSuccessful: %d - Error: %s\n", isSuccessful, errorText.c_str());
    if (isSuccessful)
        m_pClient->startApi();
}

//! [displaygrouplist]
void TestCppClient::displayGroupList( int reqId, const std::string& groups) {
	printf("Display Group List. ReqId: %d, Groups: %s\n", reqId, groups.c_str());
}
//! [displaygrouplist]

//! [displaygroupupdated]
void TestCppClient::displayGroupUpdated( int reqId, const std::string& contractInfo) {
	std::cout << "Display Group Updated. ReqId: " << reqId << ", Contract Info: " << contractInfo << std::endl;
}
//! [displaygroupupdated]

//! [positionmulti]
void TestCppClient::positionMulti( int reqId, const std::string& account,const std::string& modelCode, const Contract& contract, double pos, double avgCost) {
	printf("Position Multi. Request: %d, Account: %s, ModelCode: %s, Symbol: %s, SecType: %s, Currency: %s, Position: %g, Avg Cost: %g\n", reqId, account.c_str(), modelCode.c_str(), contract.symbol.c_str(), contract.secType.c_str(), contract.currency.c_str(), pos, avgCost);
}
//! [positionmulti]

//! [positionmultiend]
void TestCppClient::positionMultiEnd( int reqId) {
	printf("Position Multi End. Request: %d\n", reqId);
}
//! [positionmultiend]

//! [accountupdatemulti]
void TestCppClient::accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) {
	printf("AccountUpdate Multi. Request: %d, Account: %s, ModelCode: %s, Key, %s, Value: %s, Currency: %s\n", reqId, account.c_str(), modelCode.c_str(), key.c_str(), value.c_str(), currency.c_str());
}
//! [accountupdatemulti]

//! [accountupdatemultiend]
void TestCppClient::accountUpdateMultiEnd( int reqId) {
	printf("Account Update Multi End. Request: %d\n", reqId);
}
//! [accountupdatemultiend]

//! [securityDefinitionOptionParameter]
void TestCppClient::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes) {
	printf("Security Definition Optional Parameter. Request: %d, Trading Class: %s, Multiplier: %s\n", reqId, tradingClass.c_str(), multiplier.c_str());
}
//! [securityDefinitionOptionParameter]

//! [securityDefinitionOptionParameterEnd]
void TestCppClient::securityDefinitionOptionalParameterEnd(int reqId) {
	printf("Security Definition Optional Parameter End. Request: %d\n", reqId);
}
//! [securityDefinitionOptionParameterEnd]

//! [softDollarTiers]
void TestCppClient::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) {
	printf("Soft dollar tiers (%lu):", tiers.size());

	for (unsigned int i = 0; i < tiers.size(); i++) {
		printf("%s\n", tiers[i].displayName().c_str());
	}
}
//! [softDollarTiers]

//! [familyCodes]
void TestCppClient::familyCodes(const std::vector<FamilyCode> &familyCodes) {
	printf("Family codes (%lu):\n", familyCodes.size());

	for (unsigned int i = 0; i < familyCodes.size(); i++) {
		printf("Family code [%d] - accountID: %s familyCodeStr: %s\n", i, familyCodes[i].accountID.c_str(), familyCodes[i].familyCodeStr.c_str());
	}
}
//! [familyCodes]

//! [symbolSamples]
void TestCppClient::symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) {
	printf("Symbol Samples (total=%lu) reqId: %d\n", contractDescriptions.size(), reqId);

	for (unsigned int i = 0; i < contractDescriptions.size(); i++) {
		Contract contract = contractDescriptions[i].contract;
		std::vector<std::string> derivativeSecTypes = contractDescriptions[i].derivativeSecTypes;
		printf("Contract (%u): %ld %s %s %s %s, ", i, contract.conId, contract.symbol.c_str(), contract.secType.c_str(), contract.primaryExchange.c_str(), contract.currency.c_str());
		printf("Derivative Sec-types (%lu):", derivativeSecTypes.size());
		for (unsigned int j = 0; j < derivativeSecTypes.size(); j++) {
			printf(" %s", derivativeSecTypes[j].c_str());
		}
		printf("\n");
	}
}
//! [symbolSamples]

//! [mktDepthExchanges]
void TestCppClient::mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions) {
	printf("Mkt Depth Exchanges (%lu):\n", depthMktDataDescriptions.size());

	for (unsigned int i = 0; i < depthMktDataDescriptions.size(); i++) {
		printf("Depth Mkt Data Description [%d] - exchange: %s secType: %s listingExch: %s serviceDataType: %s aggGroup: %s\n", i, 
			depthMktDataDescriptions[i].exchange.c_str(), 
			depthMktDataDescriptions[i].secType.c_str(), 
			depthMktDataDescriptions[i].listingExch.c_str(), 
			depthMktDataDescriptions[i].serviceDataType.c_str(), 
			depthMktDataDescriptions[i].aggGroup != INT_MAX ? std::to_string(depthMktDataDescriptions[i].aggGroup).c_str() : "");
	}
}
//! [mktDepthExchanges]

//! [tickNews]
void TestCppClient::tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData) {
	printf("News Tick. TickerId: %d, TimeStamp: %s, ProviderCode: %s, ArticleId: %s, Headline: %s, ExtraData: %s\n", tickerId, ctime(&(timeStamp /= 1000)), providerCode.c_str(), articleId.c_str(), headline.c_str(), extraData.c_str());
}
//! [tickNews]

//! [smartcomponents]]
void TestCppClient::smartComponents(int reqId, SmartComponentsMap theMap) {
	printf("Smart components: (%lu):\n", theMap.size());

	for (SmartComponentsMap::iterator i = theMap.begin(); i != theMap.end(); i++) {
		printf(" bit number: %d exchange: %s exchange letter: %c\n", i->first, std::get<0>(i->second).c_str(), std::get<1>(i->second));
	}
}
//! [smartcomponents]

//! [tickReqParams]
void TestCppClient::tickReqParams(int tickerId, double minTick, std::string bboExchange, int snapshotPermissions) {
	printf("tickerId: %d, minTick: %g, bboExchange: %s, snapshotPermissions: %u", tickerId, minTick, bboExchange.c_str(), snapshotPermissions);

	m_bboExchange = bboExchange;
}
//! [tickReqParams]

//! [newsProviders]
void TestCppClient::newsProviders(const std::vector<NewsProvider> &newsProviders) {
	printf("News providers (%lu):\n", newsProviders.size());

	for (unsigned int i = 0; i < newsProviders.size(); i++) {
		printf("News provider [%d] - providerCode: %s providerName: %s\n", i, newsProviders[i].providerCode.c_str(), newsProviders[i].providerName.c_str());
	}
}
//! [newsProviders]

//! [newsArticle]
void TestCppClient::newsArticle(int requestId, int articleType, const std::string& articleText) {
	printf("News Article. Request Id: %d, Article Type: %d\n", requestId, articleType);
	if (articleType == 0) {
		printf("News Article Text (text or html): %s\n", articleText.c_str());
	} else if (articleType == 1) {
		printf("News Article Text (binary/pdf): Binary/pdf article text cannot be displayed\n");
	}
}
//! [newsArticle]

//! [historicalNews]
void TestCppClient::historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline) {
	printf("Historical News. RequestId: %d, Time: %s, ProviderCode: %s, ArticleId: %s, Headline: %s\n", requestId, time.c_str(), providerCode.c_str(), articleId.c_str(), headline.c_str());
}
//! [historicalNews]

//! [historicalNewsEnd]
void TestCppClient::historicalNewsEnd(int requestId, bool hasMore) {
	printf("Historical News End. RequestId: %d, HasMore: %s\n", requestId, (hasMore ? "true" : " false"));
}
//! [historicalNewsEnd]

//! [headTimestamp]
void TestCppClient::headTimestamp(int reqId, const std::string& headTimestamp) {
	printf( "Head time stamp. ReqId: %d - Head time stamp: %s,\n", reqId, headTimestamp.c_str());

}
//! [headTimestamp]

//! [histogramData]
void TestCppClient::histogramData(int reqId, HistogramDataVector data) {
	printf("Histogram. ReqId: %d, data length: %lu\n", reqId, data.size());

	for (auto item : data) {
		printf("\t price: %f, size: %lld\n", item.price, item.size);
	}
}
//! [histogramData]

//! [historicalDataUpdate]
void TestCppClient::historicalDataUpdate(TickerId reqId, Bar bar) { 
	printf( "HistoricalDataUpdate. ReqId: %ld - Date: %s, Open: %g, High: %g, Low: %g, Close: %g, Volume: %d, Count: %d, WAP: %g\n", reqId, bar.time.c_str(), bar.open, bar.high, bar.low, bar.close, bar.volume, bar.count, bar.wap);
}
//! [historicalDataUpdate]

//! [rerouteMktDataReq]
void TestCppClient::rerouteMktDataReq(int reqId, int conid, const std::string& exchange) {
	printf( "Re-route market data request. ReqId: %d, ConId: %d, Exchange: %s\n", reqId, conid, exchange.c_str());
}
//! [rerouteMktDataReq]

//! [rerouteMktDepthReq]
void TestCppClient::rerouteMktDepthReq(int reqId, int conid, const std::string& exchange) {
	printf( "Re-route market depth request. ReqId: %d, ConId: %d, Exchange: %s\n", reqId, conid, exchange.c_str());
}
//! [rerouteMktDepthReq]

//! [marketRule]
void TestCppClient::marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements) {
	printf("Market Rule Id: %d\n", marketRuleId);
	for (unsigned int i = 0; i < priceIncrements.size(); i++) {
		printf("Low Edge: %g, Increment: %g\n", priceIncrements[i].lowEdge, priceIncrements[i].increment);
	}
}
//! [marketRule]

//! [pnl]
void TestCppClient::pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL) {
	printf("PnL. ReqId: %d, daily PnL: %g, unrealized PnL: %g, realized PnL: %g\n", reqId, dailyPnL, unrealizedPnL, realizedPnL);
}
//! [pnl]

//! [pnlsingle]
void TestCppClient::pnlSingle(int reqId, int pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value) {
	printf("PnL Single. ReqId: %d, pos: %d, daily PnL: %g, unrealized PnL: %g, realized PnL: %g, value: %g\n", reqId, pos, dailyPnL, unrealizedPnL, realizedPnL, value);
}
//! [pnlsingle]

//! [historicalticks]
void TestCppClient::historicalTicks(int reqId, const std::vector<HistoricalTick>& ticks, bool done) {
    for (HistoricalTick tick : ticks) {
	    printf("Historical tick. ReqId: %d, time: %d, price: %g, size: %d\n", reqId, tick.time, tick.price, tick.size);
    }
}
//! [historicalticks]

//! [historicalticksbidask]
void TestCppClient::historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk>& ticks, bool done) {
    for (HistoricalTickBidAsk tick : ticks) {
        printf("Historical tick bid/ask. ReqId: %d, time: %d, mask: %d, price bid: %g, price ask: %g, size bid: %d, size ask: %d\n", reqId, 
            tick.time, tick.mask, tick.priceBid, tick.priceAsk, tick.sizeBid, tick.sizeAsk);
    }
}
//! [historicalticksbidask]

//! [historicaltickslast]
void TestCppClient::historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done) {
    for (HistoricalTickLast tick : ticks) {
        std::cout << "Historical tick last. ReqId: " << reqId << ", time: " << tick.time << ", mask: " << tick.mask << ", price: "<< tick.price <<
            ", size: " << tick.size << ", exchange: " << tick.exchange << ", special conditions: " << tick.specialConditions << std::endl;
    }
}
//! [historicaltickslast]

//! [tickbytickalllast]
void TestCppClient::tickByTickAllLast(int reqId, int tickType, time_t time, double price, int size, const TickAttrib& attribs, const std::string& exchange, const std::string& specialConditions) {
    printf("Tick-By-Tick. ReqId: %d, TickType: %s, Time: %s, Price: %g, Size: %d, PastLimit: %d, Unreported: %d, Exchange: %s, SpecialConditions:%s\n", 
        reqId, (tickType == 1 ? "Last" : "AllLast"), ctime(&time), price, size, attribs.pastLimit, attribs.unreported, exchange.c_str(), specialConditions.c_str());
}
//! [tickbytickalllast]

//! [tickbytickbidask]
void TestCppClient::tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, int bidSize, int askSize, const TickAttrib& attribs) {
    printf("Tick-By-Tick. ReqId: %d, TickType: BidAsk, Time: %s, BidPrice: %g, AskPrice: %g, BidSize: %d, AskSize: %d, BidPastLow: %d, AskPastHigh: %d\n", 
        reqId, ctime(&time), bidPrice, askPrice, bidSize, askSize, attribs.bidPastLow, attribs.askPastHigh);
}
//! [tickbytickbidask]

//! [tickbytickmidpoint]
void TestCppClient::tickByTickMidPoint(int reqId, time_t time, double midPoint) {
    printf("Tick-By-Tick. ReqId: %d, TickType: MidPoint, Time: %s, MidPoint: %g\n", reqId, ctime(&time), midPoint);
}
//! [tickbytickmidpoint]
