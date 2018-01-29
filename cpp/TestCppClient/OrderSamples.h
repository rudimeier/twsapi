#pragma once
#ifndef ORDERSAMPLES
#define ORDERSAMPLES

#include <vector>
#include "shared_ptr.h"

class Order;
class OrderCondition;

class OrderSamples {
public:
	static Order AtAuction(std::string action, double quantity, double price);
	static Order Discretionary(std::string action, double quantity, double price, double discretionaryAmount);
	static Order MarketOrder(std::string action, double quantity);
	static Order MarketIfTouched(std::string action, double quantity, double price);
	static Order MarketOnClose(std::string action, double quantity);
	static Order MarketOnOpen(std::string action, double quantity);
	static Order MidpointMatch(std::string action, double quantity);
	static Order PeggedToMarket(std::string action, double quantity, double marketOffset);
	static Order PeggedToStock(std::string action, double quantity, double delta, double stockReferencePrice, double startingPrice);
	static Order RelativePeggedToPrimary(std::string action, double quantity, double priceCap, double offsetAmount);
	static Order SweepToFill(std::string action, double quantity, double price);
	static Order AuctionLimit(std::string action, double quantity, double price, int auctionStrategy);
	static Order AuctionPeggedToStock(std::string action, double quantity, double startingPrice, double delta);
	static Order AuctionRelative(std::string action, double quantity, double offset);
	static Order Block(std::string action, double quantity, double price);
	static Order BoxTop(std::string action, double quantity);
	static Order LimitOrder(std::string action, double quantity, double limitPrice);
	static Order LimitIfTouched(std::string action, double quantity, double limitPrice, double triggerPrice);
	static Order LimitOnClose(std::string action, double quantity, double limitPrice);
	static Order LimitOnOpen(std::string action, double quantity, double limitPrice);
	static Order PassiveRelative(std::string action, double quantity, double offset);
	static Order PeggedToMidpoint(std::string action, double quantity, double offset);
	static void BracketOrder(int parentOrderId, Order& parent, Order& takeProfit, Order& stopLoss, std::string action, double quantity, double limitPrice, double takeProfitLimitPrice, double stopLossPrice);
	static Order MarketToLimit(std::string action, double quantity);
	static Order MarketWithProtection(std::string action, double quantity);
	static Order Stop(std::string action, double quantity, double stopPrice);
	static Order StopLimit(std::string action, double quantity, double limitPrice, double stopPrice);
	static Order StopWithProtection(std::string action, double quantity, double stopPrice);
	static Order TrailingStop(std::string action, double quantity, double trailingPercent, double trailStopPrice);
	static Order TrailingStopLimit(std::string action, double quantity, double limitPrice, double trailingAmount, double trailStopPrice);
	static Order ComboLimitOrder(std::string action, double quantity, double limitPrice, bool nonGuaranteed);
	static Order ComboMarketOrder(std::string action, double quantity, bool nonGuaranteed);
	static Order LimitOrderForComboWithLegPrices(std::string action, double quantity, std::vector<double> legprices, bool nonGuaranteed);
	static Order RelativeLimitOrder(std::string action, double quantity, double limitPrice, bool nonGuaranteed);
	static Order RelativeMarketCombo(std::string action, double quantity, bool nonGuaranteed);
	static void OneCancelsAll(std::string ocaGroup, Order& ocaOrder, int ocaType);
	static Order Volatility(std::string action, double quantity, double volatilityPercent, int volatilityType);
	static Order MarketFHedge(int parentOrderId, std::string action);
	static Order PeggedToBenchmark(std::string action, double quantity, double startingPrice, bool peggedChangeAmountDecrease, double peggedChangeAmount, double referenceChangeAmount, int referenceConId, std::string referenceExchange, double stockReferencePrice, double referenceContractLowerRange, double referenceContractUpperRange);
	static Order AttachAdjustableToStop(Order parent, double attachedOrderStopPrice, double triggerPrice, double adjustStopPrice);
	static Order AttachAdjustableToStopLimit(Order parent, double attachedOrderStopPrice, double triggerPrice, double adjustStopPrice, double adjustedStopLimitPrice);
	static OrderCondition* Price_Condition(int conId, std::string exchange, double price, bool isMore, bool isConjunction);
	static OrderCondition* Execution_Condition(std::string symbol, std::string secType, std::string exchange, bool isConjunction);
	static OrderCondition* Margin_Condition(int percent, bool isMore, bool isConjunction);
	static OrderCondition* Percent_Change_Condition(double pctChange, int conId, std::string exchange, bool isMore, bool isConjunction);
	static OrderCondition* Time_Condition(std::string time, bool isMore, bool isConjunction);
	static OrderCondition* Volume_Condition(int conId, std::string exchange, bool isMore, int volume, bool isConjunction);
};

#endif