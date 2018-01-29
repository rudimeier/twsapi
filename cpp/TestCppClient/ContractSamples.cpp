/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include "StdAfx.h"

#include "ContractSamples.h"

#include "Contract.h"

	/*
     * Contracts can be defined in multiple ways. The TWS/IB Gateway will always perform a query on the available contracts
     * and find which one is the best candidate:
     *  - More than a single candidate will yield an ambiguity error message.
     *  - No suitable candidates will produce a "contract not found" message.
     *  How do I find my contract though?
     *  - Often the quickest way is by looking for it in the TWS and looking at its description there (double click).
     *  - The TWS' symbol corresponds to the API's localSymbol. Keep this in mind when defining Futures or Options.
     *  - The TWS' underlying's symbol can usually be mapped to the API's symbol.
     *  
     * Any stock or option symbols displayed are for illustrative purposes only and are not intended to portray a recommendation.
     */

	/*
     * Usually, the easiest way to define a Stock/CASH contract is through these four attributes.
     */
Contract ContractSamples::EurGbpFx(){
	//! [cashcontract]
	Contract contract;
	contract.symbol = "EUR";
	contract.secType = "CASH";
	contract.currency = "GBP";
	contract.exchange = "IDEALPRO";
	//! [cashcontract]
	return contract;
}

Contract ContractSamples::Index(){
	//! [indcontract]
	Contract contract;
	contract.symbol = "DAX";
	contract.secType = "IND";
	contract.currency = "EUR";
	contract.exchange = "DTB";
	//! [indcontract]
	return contract;
}

Contract ContractSamples::CFD(){
	//! [cfdcontract]
	Contract contract;
	contract.symbol = "IBDE30";
	contract.secType = "CFD";
	contract.currency = "EUR";
	contract.exchange = "SMART";
	//! [cfdcontract]
	return contract;
}

Contract ContractSamples::EuropeanStock(){
	Contract contract;
    contract.symbol = "SMTPC";
    contract.secType = "STK";
    contract.currency = "EUR";
    contract.exchange = "BATEEN";
    return contract;
}

Contract ContractSamples::OptionAtIse(){
	Contract contract;
	contract.symbol = "BPX";
	contract.secType = "OPT";
	contract.currency = "USD";
	contract.exchange = "ISE";
	contract.lastTradeDateOrContractMonth = "20160916";
	contract.right = "C";
	contract.strike = 65;
	contract.multiplier = "100";
	return contract;
}

Contract ContractSamples::USStock(){
	//! [stkcontract]
	Contract contract;
	contract.symbol = "IBKR";
	contract.secType = "STK";
	contract.currency = "USD";
	//In the API side, NASDAQ is always defined as ISLAND
	contract.exchange = "ISLAND";
	//! [stkcontract]
	return contract;
}

Contract ContractSamples::USStockAtSmart(){
	Contract contract;
	contract.symbol = "IBKR";
	contract.secType = "STK";
	contract.currency = "USD";
	contract.exchange = "SMART";
	return contract;
}

Contract ContractSamples::USStockWithPrimaryExch(){
	//! [stkcontractwithprimary]
	Contract contract;
	contract.symbol = "MSFT";
	contract.secType = "STK";
	contract.currency = "USD";
	contract.exchange = "SMART";
	//Specify the Primary Exchange attribute to avoid contract ambiguity
	contract.primaryExchange = "ISLAND";
	//! [stkcontractwithprimary]
	return contract;
}

Contract ContractSamples::BondWithCusip() {
	//! [bondwithcusip]
	Contract contract;
	// enter CUSIP as symbol
	contract.symbol= "912828C57";
	contract.secType = "BOND";
	contract.exchange = "SMART";
	contract.currency = "USD";
	//! [bondwithcusip]
	return contract;
}
	
Contract ContractSamples::Bond() {
	//! [bond]
	Contract contract;
	contract.conId = 147554578;
	contract.exchange = "SMART";
	//! [bond]
	return contract;
}
	
Contract ContractSamples::MutualFund() {
	//! [fundcontract]
	Contract contract;
	contract.symbol = "VINIX";
	contract.secType = "FUND";
	contract.exchange = "FUNDSERV";
	contract.currency = "USD";
	//! [fundcontract]
	return contract;
}

Contract ContractSamples::Commodity() {
	//! [commoditycontract]
	Contract contract;
	contract.symbol = "XAUUSD";
	contract.secType = "CMDTY";
	contract.exchange = "SMART";
	contract.currency = "USD";
	//! [commoditycontract]
	return contract;
}

Contract ContractSamples::USOptionContract(){
	Contract contract;
	contract.symbol = "GOOG";
	contract.secType = "OPT";
	contract.exchange = "SMART";
	contract.currency = "USD";
	contract.lastTradeDateOrContractMonth = "20170120";
	contract.strike = 615;
	contract.right = "C";
	contract.multiplier = "100";
	return contract;
}

Contract ContractSamples::OptionAtBox(){
	//! [optcontract]
	Contract contract;
	contract.symbol = "GOOG";
	contract.secType = "OPT";
	contract.exchange = "BOX";
	contract.currency = "USD";
	contract.lastTradeDateOrContractMonth = "20170120";
	contract.strike = 615;
	contract.right = "C";
	contract.multiplier = "100";
	//! [optcontract]
	return contract;
}

	/*
	 *Option contracts require far more information since there are many contracts having the exact same
	 *attributes such as symbol, currency, strike, etc. 
	 */
Contract ContractSamples::NormalOption(){
	Contract contract;
	contract.symbol = "BAYN";
	contract.secType = "OPT";
	contract.exchange = "DTB";
	contract.currency = "EUR";
	contract.lastTradeDateOrContractMonth = "20161216";
	contract.strike = 100;
	contract.right = "C";
	contract.multiplier = "100";
	//Often, contracts will also require a trading class to rule out ambiguities
	contract.tradingClass = "BAY";
	return contract;
}

	/*
     * Option contracts require far more information since there are many contracts having the exact same
     * attributes such as symbol, currency, strike, etc. 
     */
Contract ContractSamples::OptionWithTradingClass(){
	//! [optcontract_tradingclass]
	Contract contract;
	contract.symbol = "SANT";
	contract.secType = "OPT";
	contract.exchange = "MEFFRV";
	contract.currency = "EUR";
	contract.lastTradeDateOrContractMonth = "20190621";
	contract.strike = 7.5;
	contract.right = "C";
	contract.multiplier = "100";
	contract.tradingClass = "SANEU";
	//! [optcontract_tradingclass]
	return contract;
}

/*
 * Using the contract's own symbol (localSymbol) can greatly simplify a contract description
 */
Contract ContractSamples::OptionWithLoacalSymbol(){
	//! [optcontract_localsymbol]
	Contract contract;
	//Watch out for the spaces within the local symbol!
	contract.localSymbol = "C DBK  DEC 20  1600";
	contract.secType = "OPT";
	contract.exchange = "DTB";
	contract.currency = "EUR";
	//! [optcontract_localsymbol]
	return contract;
}

	/*
     * Future contracts also require an expiration date but are less complicated than options.
     */
Contract ContractSamples::SimpleFuture(){
	//! [futcontract]
	Contract contract;
	contract.symbol = "ES";
	contract.secType = "FUT";
	contract.exchange = "GLOBEX";
	contract.currency = "USD";
	contract.lastTradeDateOrContractMonth = "201612";
	//! [futcontract]
	return contract;
}

	/*
     * Rather than giving expiration dates we can also provide the local symbol
     * attributes such as symbol, currency, strike, etc. 
     */
Contract ContractSamples::FutureWithLocalSymbol(){
	//! [futcontract_local_symbol]
	Contract contract;
	contract.secType = "FUT";
	contract.exchange = "GLOBEX";
	contract.currency = "USD";
	contract.localSymbol = "ESZ6";
	//! [futcontract_local_symbol]
	return contract;
}

Contract ContractSamples::FutureWithMultiplier(){
	//! [futcontract_multiplier]
	Contract contract;
	contract.symbol = "DAX";
	contract.secType = "FUT";
	contract.exchange = "DTB";
	contract.currency = "EUR";
	contract.lastTradeDateOrContractMonth = "201609";
	contract.multiplier = "5";
	//! [futcontract_multiplier]
	return contract;
}

	/*
     * Note the space in the symbol!
     */
Contract ContractSamples::WrongContract(){
	Contract contract;
	contract.symbol = " IJR ";
	contract.conId = 9579976;
	contract.secType = "STK";
	contract.exchange = "SMART";
	contract.currency = "USD";
	return contract;
}

Contract ContractSamples::FuturesOnOptions(){
	//! [fopcontract]
	Contract contract;
	contract.symbol = "ES";
	contract.secType = "FOP";
	contract.exchange = "GLOBEX";
	contract.currency = "USD";
	contract.lastTradeDateOrContractMonth = "20160617";
	contract.strike = 1810;
	contract.right = "C";
	contract.multiplier = "50";
	//! [fopcontract]
	return contract;
}

	/*
     * It is also possible to define contracts based on their ISIN (IBKR STK sample).
     * 
     */
Contract ContractSamples::ByISIN(){
	Contract contract;
	contract.secIdType = "ISIN";
	contract.secId = "US45841N1072";
	contract.exchange = "SMART";
	contract.currency = "USD";
	contract.secType = "STK";
	return contract;
}

	/*
     * Or their conId (EUR.USD sample).
     * Note: passing a contract containing the conId can cause problems if one of the other provided
     * attributes does not match 100% with what is in IB's database. This is particularly important
     * for contracts such as Bonds which may change their description from one day to another.
     * If the conId is provided, it is best not to give too much information as in the example below.
     */
Contract ContractSamples::ByConId(){
	Contract contract;
	contract.conId = 12087792;
	contract.exchange = "IDEALPRO";
	contract.secType = "CASH";
	return contract;
}

	/*
     * Ambiguous contracts are great to use with reqContractDetails. This way you can
     * query the whole option chain for an underlying. Bear in mind that there are
     * pacing mechanisms in place which will delay any further responses from the TWS
     * to prevent abuse.
     */
Contract ContractSamples::OptionForQuery(){
	//! [optionforquery]
	Contract contract;
	contract.symbol = "FISV";
	contract.secType = "OPT";
	contract.exchange = "SMART";
	contract.currency = "USD";
	//! [optionforquery]
	return contract;
}

Contract ContractSamples::OptionComboContract(){
	//! [bagoptcontract]
	Contract contract;
	contract.symbol = "DBK";
	contract.secType = "BAG";
	contract.currency = "EUR";
	contract.exchange = "DTB";

	ComboLegSPtr leg1(new ComboLeg);
	leg1->conId = 197397509;
	leg1->action = "BUY";
	leg1->ratio = 1;
	leg1->exchange = "DTB";

	ComboLegSPtr leg2(new ComboLeg);
	leg2->conId = 197397584;
	leg2->action = "SELL";
	leg2->ratio = 1;
	leg2->exchange = "DTB";

	contract.comboLegs.reset(new Contract::ComboLegList());
	contract.comboLegs->push_back(leg1);
	contract.comboLegs->push_back(leg2);
	//! [bagoptcontract]
	return contract;
}

	/*
     * STK Combo contract
     * Leg 1: 43645865 - IBKR's STK
     * Leg 2: 9408 - McDonald's STK
     */
Contract ContractSamples::StockComboContract(){
	//! [bagstkcontract]
	Contract contract;
	contract.symbol = "MCD";
	contract.secType = "BAG";
	contract.currency = "USD";
	contract.exchange = "SMART";

	ComboLegSPtr leg1(new ComboLeg);
	leg1->conId = 43645865;
	leg1->action = "BUY";
	leg1->ratio = 1;
	leg1->exchange = "SMART";

	ComboLegSPtr leg2(new ComboLeg);
	leg2->conId = 9408;
	leg2->action = "SELL";
	leg2->ratio = 1;
	leg2->exchange = "SMART";

	contract.comboLegs.reset(new Contract::ComboLegList());
	contract.comboLegs->push_back(leg1);
	contract.comboLegs->push_back(leg2);
	//! [bagstkcontract]
	return contract;
}

	/*
     * CBOE Volatility Index Future combo contract
     * Leg 1: 195538625 - FUT expiring 2016/02/17
     * Leg 2: 197436571 - FUT expiring 2016/03/16
     */
Contract ContractSamples::FutureComboContract(){
	//! [bagfutcontract]
	Contract contract;
	contract.symbol = "VIX";
	contract.secType = "BAG";
	contract.currency = "USD";
	contract.exchange = "CFE";

	ComboLegSPtr leg1(new ComboLeg);
	leg1->conId = 195538625;
	leg1->action = "BUY";
	leg1->ratio = 1;
	leg1->exchange = "CFE";

	ComboLegSPtr leg2(new ComboLeg);
	leg2->conId = 197436571;
	leg2->action = "SELL";
	leg2->ratio = 1;
	leg2->exchange = "CFE";

	contract.comboLegs.reset(new Contract::ComboLegList());
	contract.comboLegs->push_back(leg1);
	contract.comboLegs->push_back(leg2);
	//! [bagfutcontract]
	return contract;
}

Contract ContractSamples::InterCmdtyFuturesContract(){
	//! [intcmdfutcontract]
	Contract contract;
	contract.symbol = "CL.BZ";
	contract.secType = "BAG";
	contract.currency = "USD";
	contract.exchange = "NYMEX";

	ComboLegSPtr leg1(new ComboLeg);
	leg1->conId = 47207310; //CL Dec'16 @NYMEX
	leg1->action = "BUY";
	leg1->ratio = 1;
	leg1->exchange = "NYMEX";

	ComboLegSPtr leg2(new ComboLeg);
	leg2->conId = 47195961; //BZ Dec'16 @NYMEX
	leg2->action = "SELL";
	leg2->ratio = 1;
	leg2->exchange = "NYMEX";

	contract.comboLegs.reset(new Contract::ComboLegList());
	contract.comboLegs->push_back(leg1);
	contract.comboLegs->push_back(leg2);
	//! [intcmdfutcontract]
	return contract;
}

Contract ContractSamples::NewsFeedForQuery()
{
	//! [newsfeedforquery]
	Contract contract;
	contract.secType = "NEWS";
	contract.exchange = "BT"; //Briefing Trader
	//! [newsfeedforquery]
	return contract;
}

Contract ContractSamples::BTbroadtapeNewsFeed()
{
	//! [newscontractbt]
	Contract contract;
	contract.symbol = "BT:BT_ALL"; //BroadTape All News
	contract.secType = "NEWS";
	contract.exchange = "BT"; //Briefing Trader
	//! [newscontractbt]
	return contract;
}

Contract ContractSamples::BZbroadtapeNewsFeed()
{
	//! [newscontractbz]
	Contract contract;
	contract.symbol = "BZ:BZ_ALL"; //BroadTape All News
	contract.secType = "NEWS";
	contract.exchange = "BZ"; //Benzinga Pro
	//! [newscontractbz]
	return contract;
}

Contract ContractSamples::FLYbroadtapeNewsFeed()
{
	//! [newscontractfly]
	Contract contract;
	contract.symbol = "FLY:FLY_ALL"; //BroadTape All News
	contract.secType = "NEWS";
	contract.exchange = "FLY"; //Fly on the Wall
							   //! [newscontractfly]
	return contract;
}

Contract ContractSamples::MTbroadtapeNewsFeed()
{
	//! [newscontractmt]
	Contract contract;
	contract.symbol = "MT:MT_ALL"; //BroadTape All News
	contract.secType = "NEWS";
	contract.exchange = "MT"; //Midnight Trader
	//! [newscontractmt]
	return contract;
}