/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include "StdAfx.h"

#include "OrderSamples.h"
#include "Order.h"
#include "PriceCondition.h"
#include "executioncondition.h"
#include "MarginCondition.h"
#include "PercentChangeCondition.h"
#include "TimeCondition.h"
#include "VolumeCondition.h"

	/// <summary>
    /// Make sure to test using only your paper trading account when applicable. A good way of finding out if an order type/exchange combination
    /// is possible is by trying to place such order manually using the TWS.
    /// Before contacting our API support team please refer to the available documentation.
    /// </summary>

	/// <summary>
    /// An auction order is entered into the electronic trading system during the pre-market opening period for execution at the 
    /// Calculated Opening Price (COP). If your order is not filled on the open, the order is re-submitted as a limit order with 
    /// the limit price set to the COP or the best bid/ask after the market opens.
    /// Products: FUT, STK
    /// </summary>
Order OrderSamples::AtAuction(std::string action, double quantity, double price){
	//! [auction]
	Order order;
	order.action = action;
	order.tif = "AUC";
	order.orderType = "MTL";
	order.totalQuantity = quantity;
	order.lmtPrice = price;
	//! [auction]
	return order;
}

	/// <summary>
    /// A Discretionary order is a limit order submitted with a hidden, specified 'discretionary' amount off the limit price which
    /// may be used to increase the price range over which the limit order is eligible to execute. The market sees only the limit price.
    /// Products: STK
    /// </summary>
Order OrderSamples::Discretionary(std::string action, double quantity, double price, double discretionaryAmount){
	//! [discretionary]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = price;
	order.discretionaryAmt = discretionaryAmount;
	//! [discretionary]
	return order;
}

	/// <summary>
    /// A Market order is an order to buy or sell at the market bid or offer price. A market order may increase the likelihood of a fill 
    /// and the speed of execution, but unlike the Limit order a Market order provides no price protection and may fill at a price far 
    /// lower/higher than the current displayed bid/ask.
    /// Products: BOND, CFD, EFP, CASH, FUND, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::MarketOrder(std::string action, double quantity){
	//! [market]
	Order order;
	order.action = action;
	order.orderType = "MKT";
	order.totalQuantity = quantity;
	//! [market]
	return order;
}

	/// <summary>
    /// A Market if Touched (MIT) is an order to buy (or sell) a contract below (or above) the market. Its purpose is to take advantage 
    /// of sudden or unexpected changes in share or other prices and provides investors with a trigger price to set an order in motion. 
    /// Investors may be waiting for excessive strength (or weakness) to cease, which might be represented by a specific price point. 
    /// MIT orders can be used to determine whether or not to enter the market once a specific price level has been achieved. This order 
    /// is held in the system until the trigger price is touched, and is then submitted as a market order. An MIT order is similar to a 
    /// stop order, except that an MIT sell order is placed above the current market price, and a stop sell order is placed below
    /// Products: BOND, CFD, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::MarketIfTouched(std::string action, double quantity, double price){
	//! [market_if_touched]
	Order order;
	order.action = action;
	order.orderType = "MIT";
	order.totalQuantity = quantity;
	order.auxPrice = price;
	//! [market_if_touched]
	return order;
}

	/// <summary>
    /// A Market-on-Close (MOC) order is a market order that is submitted to execute as close to the closing price as possible.
    /// Products: CFD, FUT, STK, WAR
    /// </summary>
Order OrderSamples::MarketOnClose(std::string action, double quantity){
	//! [market_on_close]
	Order order;
	order.action = action;
	order.orderType = "MOC";
	order.totalQuantity = quantity;
	//! [market_on_close]
	return order;
}

	/// <summary>
    /// A Market-on-Open (MOO) order combines a market order with the OPG time in force to create an order that is automatically
    /// submitted at the market's open and fills at the market price.
    /// Products: CFD, STK, OPT, WAR
    /// </summary>
Order OrderSamples::MarketOnOpen(std::string action, double quantity){
	//! [market_on_open]
	Order order;
	order.action = action;
	order.orderType = "MKT";
	order.totalQuantity = quantity;
	order.tif = "OPG";
	//! [market_on_open]
	return order;
}

	/// <summary>
    /// ISE Midpoint Match (MPM) orders always execute at the midpoint of the NBBO. You can submit market and limit orders direct-routed 
    /// to ISE for MPM execution. Market orders execute at the midpoint whenever an eligible contra-order is available. Limit orders 
    /// execute only when the midpoint price is better than the limit price. Standard MPM orders are completely anonymous.
    /// Products: STK
    /// </summary>
Order OrderSamples::MidpointMatch(std::string action, double quantity){
	//! [midpoint_match]
	Order order;
	order.action = action;
	order.orderType = "MKT";
	order.totalQuantity = quantity;
	//! [midpoint_match]
	return order;
}

	/// <summary>
    /// A pegged-to-market order is designed to maintain a purchase price relative to the national best offer (NBO) or a sale price 
    /// relative to the national best bid (NBB). Depending on the width of the quote, this order may be passive or aggressive. 
    /// The trader creates the order by entering a limit price which defines the worst limit price that they are willing to accept. 
    /// Next, the trader enters an offset amount which computes the active limit price as follows:
    ///     Sell order price = Bid price + offset amount
    ///     Buy order price = Ask price - offset amount
    /// Products: STK
    /// </summary>
Order OrderSamples::PeggedToMarket(std::string action, double quantity, double marketOffset){
	//! [pegged_market]
	Order order;
	order.action = action;
	order.orderType = "PEG MKT";
	order.totalQuantity = quantity;
	order.auxPrice = marketOffset;
	//! [pegged_market]
	return order;
}

	/// <summary>
    /// A Pegged to Stock order continually adjusts the option order price by the product of a signed user-define delta and the change of 
    /// the option's underlying stock price. The delta is entered as an absolute and assumed to be positive for calls and negative for puts. 
    /// A buy or sell call order price is determined by adding the delta times a change in an underlying stock price to a specified starting 
    /// price for the call. To determine the change in price, the stock reference price is subtracted from the current NBBO midpoint. 
    /// The Stock Reference Price can be defined by the user, or defaults to the NBBO midpoint at the time of the order if no reference price 
    /// is entered. You may also enter a high/low stock price range which cancels the order when reached. The delta times the change in stock 
    /// price will be rounded to the nearest penny in favor of the order.
    /// Products: OPT
    /// </summary>
Order OrderSamples::PeggedToStock(std::string action, double quantity, double delta, double stockReferencePrice, double startingPrice){
	//! [pegged_stock]
	Order order;
	order.action = action;
	order.orderType = "PEG STK";
	order.totalQuantity = quantity;
	order.delta = delta;
	order.lmtPrice = stockReferencePrice;
	order.startingPrice = startingPrice;
	//! [pegged_stock]
	return order;
}

	/// <summary>
    /// Relative (a.k.a. Pegged-to-Primary) orders provide a means for traders to seek a more aggressive price than the National Best Bid 
    /// and Offer (NBBO). By acting as liquidity providers, and placing more aggressive bids and offers than the current best bids and offers, 
    /// traders increase their odds of filling their order. Quotes are automatically adjusted as the markets move, to remain aggressive. 
    /// For a buy order, your bid is pegged to the NBB by a more aggressive offset, and if the NBB moves up, your bid will also move up. 
    /// If the NBB moves down, there will be no adjustment because your bid will become even more aggressive and execute. For sales, your 
    /// offer is pegged to the NBO by a more aggressive offset, and if the NBO moves down, your offer will also move down. If the NBO moves up, 
    /// there will be no adjustment because your offer will become more aggressive and execute. In addition to the offset, you can define an 
    /// absolute cap, which works like a limit price, and will prevent your order from being executed above or below a specified level.
    /// Stocks, Options and Futures - not available on paper trading
    /// Products: CFD, STK, OPT, FUT
    /// </summary>
Order OrderSamples::RelativePeggedToPrimary(std::string action, double quantity, double priceCap, double offsetAmount){
	//! [relative_pegged_primary]
	Order order;
	order.action = action;
	order.orderType = "REL";
	order.totalQuantity = quantity;
	order.lmtPrice = priceCap;
	order.auxPrice = offsetAmount;
	//! [relative_pegged_primary]
	return order;
}

	/// <summary>
    /// Sweep-to-fill orders are useful when a trader values speed of execution over price. A sweep-to-fill order identifies the best price 
    /// and the exact quantity offered/available at that price, and transmits the corresponding portion of your order for immediate execution. 
    /// Simultaneously it identifies the next best price and quantity offered/available, and submits the matching quantity of your order for 
    /// immediate execution.
    /// Products: CFD, STK, WAR
    /// Supported Exchanges: SMART
    /// </summary>
Order OrderSamples::SweepToFill(std::string action, double quantity, double price){
	//! [sweep_to_fill]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = price;
	order.sweepToFill = true;
	//! [sweep_to_fill]
	return order;
}

	/// <summary>
    /// For option orders routed to the Boston Options Exchange (BOX) you may elect to participate in the BOX's price improvement auction in 
    /// pennies. All BOX-directed price improvement orders are immediately sent from Interactive Brokers to the BOX order book, and when the 
    /// terms allow, IB will evaluate it for inclusion in a price improvement auction based on price and volume priority. In the auction, your 
    /// order will have priority over broker-dealer price improvement orders at the same price.
    /// An Auction Limit order at a specified price. Use of a limit order ensures that you will not receive an execution at a price less favorable 
    /// than the limit price. Enter limit orders in penny increments with your auction improvement amount computed as the difference between your 
    /// limit order price and the nearest listed increment.
    /// Products: OPT
    /// Supported Exchanges: BOX
    /// </summary>
Order OrderSamples::AuctionLimit(std::string action, double quantity, double price, int auctionStrategy){
	//! [auction_limit]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = price;
	order.auctionStrategy = auctionStrategy;
	//! [auction_limit]
	return order;
}

	/// <summary>
    /// For option orders routed to the Boston Options Exchange (BOX) you may elect to participate in the BOX's price improvement auction in pennies. 
    /// All BOX-directed price improvement orders are immediately sent from Interactive Brokers to the BOX order book, and when the terms allow, 
    /// IB will evaluate it for inclusion in a price improvement auction based on price and volume priority. In the auction, your order will have 
    /// priority over broker-dealer price improvement orders at the same price.
    /// An Auction Pegged to Stock order adjusts the order price by the product of a signed delta (which is entered as an absolute and assumed to be 
    /// positive for calls, negative for puts) and the change of the option's underlying stock price. A buy or sell call order price is determined 
    /// by adding the delta times a change in an underlying stock price change to a specified starting price for the call. To determine the change 
    /// in price, a stock reference price (NBBO midpoint at the time of the order is assumed if no reference price is entered) is subtracted from 
    /// the current NBBO midpoint. A stock range may also be entered that cancels an order when reached. The delta times the change in stock price 
    /// will be rounded to the nearest penny in favor of the order and will be used as your auction improvement amount.
    /// Products: OPT
    /// Supported Exchanges: BOX
    /// </summary>
Order OrderSamples::AuctionPeggedToStock(std::string action, double quantity, double startingPrice, double delta){
	//! [auction_pegged_stock]
	Order order;
	order.action = action;
	order.orderType = "PEG STK";
	order.totalQuantity = quantity;
	order.delta = delta;
	order.startingPrice = startingPrice;
	//! [auction_pegged_stock]
	return order;
}

	/// <summary>
    /// For option orders routed to the Boston Options Exchange (BOX) you may elect to participate in the BOX's price improvement auction in pennies. 
    /// All BOX-directed price improvement orders are immediately sent from Interactive Brokers to the BOX order book, and when the terms allow, 
    /// IB will evaluate it for inclusion in a price improvement auction based on price and volume priority. In the auction, your order will have 
    /// priority over broker-dealer price improvement orders at the same price.
    /// An Auction Relative order that adjusts the order price by the product of a signed delta (which is entered as an absolute and assumed to be 
    /// positive for calls, negative for puts) and the change of the option's underlying stock price. A buy or sell call order price is determined 
    /// by adding the delta times a change in an underlying stock price change to a specified starting price for the call. To determine the change 
    /// in price, a stock reference price (NBBO midpoint at the time of the order is assumed if no reference price is entered) is subtracted from 
    /// the current NBBO midpoint. A stock range may also be entered that cancels an order when reached. The delta times the change in stock price 
    /// will be rounded to the nearest penny in favor of the order and will be used as your auction improvement amount.
    /// Products: OPT
    /// Supported Exchanges: BOX
    /// </summary>
Order OrderSamples::AuctionRelative(std::string action, double quantity, double offset){
	//! [auction_relative]
	Order order;
	order.action = action;
	order.orderType = "REL";
	order.totalQuantity = quantity;
	order.auxPrice = offset;
	//! [auction_relative]
	return order;
}

	/// <summary>
    /// The Block attribute is used for large volume option orders on ISE that consist of at least 50 contracts. To execute large-volume 
    /// orders over time without moving the market, use the Accumulate/Distribute algorithm.
    /// Products: OPT
    /// </summary>
Order OrderSamples::Block(std::string action, double quantity, double price){
	// ! [block]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = price;
	order.blockOrder = true;
	// ! [block]
	return order;
}

	/// <summary>
    /// A Box Top order executes as a market order at the current best price. If the order is only partially filled, the remainder is submitted as 
    /// a limit order with the limit price equal to the price at which the filled portion of the order executed.
    /// Products: OPT
    /// Supported Exchanges: BOX
    /// </summary>
Order OrderSamples::BoxTop(std::string action, double quantity){
	// ! [boxtop]
	Order order;
	order.action = action;
	order.orderType = "BOX TOP";
	order.totalQuantity = quantity;
	// ! [boxtop]
	return order;
}

	/// <summary>
    /// A Limit order is an order to buy or sell at a specified price or better. The Limit order ensures that if the order fills, 
    /// it will not fill at a price less favorable than your limit price, but it does not guarantee a fill.
    /// Products: BOND, CFD, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::LimitOrder(std::string action, double quantity, double limitPrice){
	// ! [limitorder]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	// ! [limitorder]
	return order;
}

	/// <summary>
    /// A Limit if Touched is an order to buy (or sell) a contract at a specified price or better, below (or above) the market. This order is 
    /// held in the system until the trigger price is touched. An LIT order is similar to a stop limit order, except that an LIT sell order is 
    /// placed above the current market price, and a stop limit sell order is placed below.
    /// Products: BOND, CFD, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::LimitIfTouched(std::string action, double quantity, double limitPrice, double triggerPrice){
	// ! [limitiftouched]
	Order order;
	order.action = action;
	order.orderType = "LIT";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	order.auxPrice = triggerPrice;
	// ! [limitiftouched]
	return order;
}

	/// <summary>
    /// A Limit-on-close (LOC) order will be submitted at the close and will execute if the closing price is at or better than the submitted 
    /// limit price.
    /// Products: CFD, FUT, STK, WAR
    /// </summary>
Order OrderSamples::LimitOnClose(std::string action, double quantity, double limitPrice){
	// ! [limitonclose]
	Order order;
	order.action = action;
	order.orderType = "LOC";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	// ! [limitonclose]
	return order;
}

	/// <summary>
    /// A Limit-on-Open (LOO) order combines a limit order with the OPG time in force to create an order that is submitted at the market's open, 
    /// and that will only execute at the specified limit price or better. Orders are filled in accordance with specific exchange rules.
    /// Products: CFD, STK, OPT, WAR
    /// </summary>
Order OrderSamples::LimitOnOpen(std::string action, double quantity, double limitPrice){
	// ! [limitonopen]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.tif = "OPG";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	// ! [limitonopen]
	return order;
}

	/// <summary>
    /// Passive Relative orders provide a means for traders to seek a less aggressive price than the National Best Bid and Offer (NBBO) while 
    /// keeping the order pegged to the best bid (for a buy) or ask (for a sell). The order price is automatically adjusted as the markets move 
    /// to keep the order less aggressive. For a buy order, your order price is pegged to the NBB by a less aggressive offset, and if the NBB 
    /// moves up, your bid will also move up. If the NBB moves down, there will be no adjustment because your bid will become aggressive and execute. 
    /// For a sell order, your price is pegged to the NBO by a less aggressive offset, and if the NBO moves down, your offer will also move down. 
    /// If the NBO moves up, there will be no adjustment because your offer will become aggressive and execute. In addition to the offset, you can 
    /// define an absolute cap, which works like a limit price, and will prevent your order from being executed above or below a specified level. 
    /// The Passive Relative order is similar to the Relative/Pegged-to-Primary order, except that the Passive relative subtracts the offset from 
    /// the bid and the Relative adds the offset to the bid.
    /// Products: STK, WAR
    /// </summary>
Order OrderSamples::PassiveRelative(std::string action, double quantity, double offset){
	// ! [passive_relative]
	Order order;
	order.action = action;
	order.orderType = "PASSV REL";
	order.totalQuantity = quantity;
	order.auxPrice = offset;
	// ! [passive_relative]
	return order;
}

	/// <summary>
    /// A pegged-to-midpoint order provides a means for traders to seek a price at the midpoint of the National Best Bid and Offer (NBBO). 
    /// The price automatically adjusts to peg the midpoint as the markets move, to remain aggressive. For a buy order, your bid is pegged to 
    /// the NBBO midpoint and the order price adjusts automatically to continue to peg the midpoint if the market moves. The price only adjusts 
    /// to be more aggressive. If the market moves in the opposite direction, the order will execute.
    /// Products: STK
    /// </summary>
Order OrderSamples::PeggedToMidpoint(std::string action, double quantity, double offset){
	// ! [pegged_midpoint]
	Order order;
	order.action = action;
	order.orderType = "PEG MID";
	order.totalQuantity = quantity;
	order.auxPrice = offset;
	// ! [pegged_midpoint]
	return order;
}

	/// <summary>
    /// Bracket orders are designed to help limit your loss and lock in a profit by "bracketing" an order with two opposite-side orders. 
    /// A BUY order is bracketed by a high-side sell limit order and a low-side sell stop order. A SELL order is bracketed by a high-side buy 
    /// stop order and a low side buy limit order.
    /// Products: CFD, BAG, FOP, CASH, FUT, OPT, STK, WAR
    /// </summary>	
	//! [bracket]
void OrderSamples::BracketOrder(int parentOrderId, Order& parent, Order& takeProfit, Order& stopLoss, std::string action, double quantity, double limitPrice, double takeProfitLimitPrice, double stopLossPrice){
	//This will be our main or "parent" order
	parent.orderId = parentOrderId;
	parent.action = action;
	parent.orderType = "LMT";
	parent.totalQuantity = quantity;
	parent.lmtPrice = limitPrice;
	//The parent and children orders will need this attribute set to false to prevent accidental executions.
    //The LAST CHILD will have it set to true, 
	parent.transmit = false;

	takeProfit.orderId = parent.orderId + 1;
	takeProfit.action = (action == "BUY") ? "SELL" : "BUY";
	takeProfit.orderType = "LMT";
	takeProfit.totalQuantity = quantity;
	takeProfit.lmtPrice = takeProfitLimitPrice;
	takeProfit.parentId = parentOrderId;
	takeProfit.transmit = false;

	stopLoss.orderId = parent.orderId + 2;
	stopLoss.action = (action == "BUY") ? "SELL" : "BUY";
	stopLoss.orderType = "STP";
	//Stop trigger price
	stopLoss.auxPrice = stopLossPrice;
	stopLoss.totalQuantity = quantity;
	stopLoss.parentId = parentOrderId;
	//In this case, the low side order will be the last child being sent. Therefore, it needs to set this attribute to true 
    //to activate all its predecessors
	stopLoss.transmit = true;
}
	//! [bracket]

	/// <summary>
    /// Products:CFD, FUT, FOP, OPT, STK, WAR
    /// A Market-to-Limit (MTL) order is submitted as a market order to execute at the current best market price. If the order is only 
    /// partially filled, the remainder of the order is canceled and re-submitted as a limit order with the limit price equal to the price 
    /// at which the filled portion of the order executed.
    /// </summary>
Order OrderSamples::MarketToLimit(std::string action, double quantity){
	// ! [markettolimit]
	Order order;
	order.action = action;
	order.orderType = "MTL";
	order.totalQuantity = quantity;
	// ! [markettolimit]
	return order;
}

	/// <summary>
    /// This order type is useful for futures traders using Globex. A Market with Protection order is a market order that will be cancelled and 
    /// resubmitted as a limit order if the entire order does not immediately execute at the market price. The limit price is set by Globex to be 
    /// close to the current market price, slightly higher for a sell order and lower for a buy order.
    /// Products: FUT, FOP
    /// </summary>
Order OrderSamples::MarketWithProtection(std::string action, double quantity){
	// ! [marketwithprotection]
	Order order;
	order.action = action;
	order.orderType = "MKT PRT";
	order.totalQuantity = quantity;
	// ! [marketwithprotection]
	return order;
}

	/// <summary>
    /// A Stop order is an instruction to submit a buy or sell market order if and when the user-specified stop trigger price is attained or 
    /// penetrated. A Stop order is not guaranteed a specific execution price and may execute significantly away from its stop price. A Sell 
    /// Stop order is always placed below the current market price and is typically used to limit a loss or protect a profit on a long stock 
    /// position. A Buy Stop order is always placed above the current market price. It is typically used to limit a loss or help protect a 
    /// profit on a short sale.
    /// Products: CFD, BAG, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::Stop(std::string action, double quantity, double stopPrice){
	// ! [stop]
	Order order;
	order.action = action;
	order.orderType = "STP";
	order.totalQuantity = quantity;
	order.auxPrice = stopPrice;
	// ! [stop]
	return order;
}

	/// <summary>
    /// A Stop-Limit order is an instruction to submit a buy or sell limit order when the user-specified stop trigger price is attained or 
    /// penetrated. The order has two basic components: the stop price and the limit price. When a trade has occurred at or through the stop 
    /// price, the order becomes executable and enters the market as a limit order, which is an order to buy or sell at a specified price or better.
    /// Products: CFD, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::StopLimit(std::string action, double quantity, double limitPrice, double stopPrice){
	// ! [stoplimit]
	Order order;
	order.action = action;
	order.orderType = "STP LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	order.auxPrice = stopPrice;
	// ! [stoplimit]
	return order;
}

	/// <summary>
    /// A Stop with Protection order combines the functionality of a stop limit order with a market with protection order. The order is set 
    /// to trigger at a specified stop price. When the stop price is penetrated, the order is triggered as a market with protection order, 
    /// which means that it will fill within a specified protected price range equal to the trigger price +/- the exchange-defined protection 
    /// point range. Any portion of the order that does not fill within this protected range is submitted as a limit order at the exchange-defined 
    /// trigger price +/- the protection points.
    /// Products: FUT
    /// </summary>
Order OrderSamples::StopWithProtection(std::string action, double quantity, double stopPrice){
	// ! [stopwithprotection]
	Order order;
	order.action = action;
	order.orderType = "STP PRT";
	order.totalQuantity = quantity;
	order.auxPrice = stopPrice;
	// ! [stopwithprotection]
	return order;
}

	/// <summary>
    /// A sell trailing stop order sets the stop price at a fixed amount below the market price with an attached "trailing" amount. As the 
    /// market price rises, the stop price rises by the trail amount, but if the stock price falls, the stop loss price doesn't change, 
    /// and a market order is submitted when the stop price is hit. This technique is designed to allow an investor to specify a limit on the 
    /// maximum possible loss, without setting a limit on the maximum possible gain. "Buy" trailing stop orders are the mirror image of sell 
    /// trailing stop orders, and are most appropriate for use in falling markets.
    /// Products: CFD, CASH, FOP, FUT, OPT, STK, WAR
    /// </summary>
Order OrderSamples::TrailingStop(std::string action, double quantity, double trailingPercent, double trailStopPrice){
	// ! [trailingstop]
	Order order;
	order.action = action;
	order.orderType = "TRAIL";
	order.totalQuantity = quantity;
	order.trailingPercent = trailingPercent;
	order.trailStopPrice = trailStopPrice;
	// ! [trailingstop]
	return order;
}

	/// <summary>
    /// A trailing stop limit order is designed to allow an investor to specify a limit on the maximum possible loss, without setting a limit 
    /// on the maximum possible gain. A SELL trailing stop limit moves with the market price, and continually recalculates the stop trigger 
    /// price at a fixed amount below the market price, based on the user-defined "trailing" amount. The limit order price is also continually 
    /// recalculated based on the limit offset. As the market price rises, both the stop price and the limit price rise by the trail amount and 
    /// limit offset respectively, but if the stock price falls, the stop price remains unchanged, and when the stop price is hit a limit order 
    /// is submitted at the last calculated limit price. A "Buy" trailing stop limit order is the mirror image of a sell trailing stop limit, 
    /// and is generally used in falling markets.
    /// Products: BOND, CFD, CASH, FUT, FOP, OPT, STK, WAR
    /// </summary>
Order OrderSamples::TrailingStopLimit(std::string action, double quantity, double limitPrice, double trailingAmount, double trailStopPrice){
	// ! [trailingstoplimit]
	Order order;
	order.action = action;
	order.orderType = "TRAIL LIMIT";
	order.totalQuantity = quantity;
	order.trailStopPrice = trailStopPrice;
	order.lmtPrice = limitPrice;
	order.auxPrice = trailingAmount;
	// ! [trailingstoplimit]
	return order;
}

	/// <summary>
    /// Create combination orders that include options, stock and futures legs (stock legs can be included if the order is routed 
    /// through SmartRouting). Although a combination/spread order is constructed of separate legs, it is executed as a single transaction 
    /// if it is routed directly to an exchange. For combination orders that are SmartRouted, each leg may be executed separately to ensure 
    /// best execution.
    /// Products: OPT, STK, FUT
    /// </summary>
Order OrderSamples::ComboLimitOrder(std::string action, double quantity, double limitPrice, bool nonGuaranteed){
	// ! [combolimit]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.lmtPrice = limitPrice;
	if(nonGuaranteed){
		TagValueSPtr tag1(new TagValue("NonGuaranteed", "1"));
		order.smartComboRoutingParams.reset(new TagValueList());
		order.smartComboRoutingParams->push_back(tag1);
	}
	// ! [combolimit]
	return order;
}

	/// <summary>
    /// Create combination orders that include options, stock and futures legs (stock legs can be included if the order is routed 
    /// through SmartRouting). Although a combination/spread order is constructed of separate legs, it is executed as a single transaction 
    /// if it is routed directly to an exchange. For combination orders that are SmartRouted, each leg may be executed separately to ensure 
    /// best execution.
    /// Products: OPT, STK, FUT
    /// </summary>
Order OrderSamples::ComboMarketOrder(std::string action, double quantity, bool nonGuaranteed){
	// ! [combomarket]
	Order order;
	order.action = action;
	order.orderType = "MKT";
	order.totalQuantity = quantity;
	if(nonGuaranteed){
		TagValueSPtr tag1(new TagValue("NonGuaranteed", "1"));
		order.smartComboRoutingParams.reset(new TagValueList());
		order.smartComboRoutingParams->push_back(tag1);
	}
	// ! [combomarket]
	return order;
}

	/// <summary>
    /// Create combination orders that include options, stock and futures legs (stock legs can be included if the order is routed 
    /// through SmartRouting). Although a combination/spread order is constructed of separate legs, it is executed as a single transaction 
    /// if it is routed directly to an exchange. For combination orders that are SmartRouted, each leg may be executed separately to ensure 
    /// best execution.
    /// Products: OPT, STK, FUT
    /// </summary>
Order OrderSamples::LimitOrderForComboWithLegPrices(std::string action, double quantity, std::vector<double> legprices, bool nonGuaranteed){
	// ! [limitordercombolegprices]
	Order order;
	order.action = action;
	order.orderType = "LMT";
	order.totalQuantity = quantity;
	order.orderComboLegs.reset(new Order::OrderComboLegList());
	for(unsigned int i = 0; i < legprices.size(); i++){
		OrderComboLegSPtr comboLeg(new OrderComboLeg());
		comboLeg->price = legprices[i];
		order.orderComboLegs->push_back(comboLeg);
	}
	if(nonGuaranteed){
		TagValueSPtr tag1(new TagValue("NonGuaranteed", "1"));
		order.smartComboRoutingParams.reset(new TagValueList());
		order.smartComboRoutingParams->push_back(tag1);
	}
	// ! [limitordercombolegprices]
	return order;
}

	/// <summary>
    /// Create combination orders that include options, stock and futures legs (stock legs can be included if the order is routed 
    /// through SmartRouting). Although a combination/spread order is constructed of separate legs, it is executed as a single transaction 
    /// if it is routed directly to an exchange. For combination orders that are SmartRouted, each leg may be executed separately to ensure 
    /// best execution.
    /// Products: OPT, STK, FUT
    /// </summary>
Order OrderSamples::RelativeLimitOrder(std::string action, double quantity, double limitPrice, bool nonGuaranteed){
	// ! [relativelimitcombo]
	Order order;
	order.action = action;
	order.totalQuantity = quantity;
	order.orderType = "Rel + LMT";
	order.lmtPrice = limitPrice;
	if(nonGuaranteed){
		TagValueSPtr tag1(new TagValue("NonGuaranteed", "1"));
		order.smartComboRoutingParams.reset(new TagValueList());
		order.smartComboRoutingParams->push_back(tag1);
	}
	// ! [relativelimitcombo]
	return order;
}

	/// <summary>
    /// Create combination orders that include options, stock and futures legs (stock legs can be included if the order is routed 
    /// through SmartRouting). Although a combination/spread order is constructed of separate legs, it is executed as a single transaction 
    /// if it is routed directly to an exchange. For combination orders that are SmartRouted, each leg may be executed separately to ensure 
    /// best execution.
    /// Products: OPT, STK, FUT
    /// </summary>
Order OrderSamples::RelativeMarketCombo(std::string action, double quantity, bool nonGuaranteed){
	// ! [relativemarketcombo]
	Order order;
	order.action = action;
	order.totalQuantity = quantity;
	order.orderType = "Rel + MKT";
	if(nonGuaranteed){
		TagValueSPtr tag1(new TagValue("NonGuaranteed", "1"));
		order.smartComboRoutingParams.reset(new TagValueList());
		order.smartComboRoutingParams->push_back(tag1);
	}
	// ! [relativemarketcombo]
	return order;
}

	/// <summary>
    /// One-Cancels All (OCA) order type allows an investor to place multiple and possibly unrelated orders assigned to a group. The aim is 
    /// to complete just one of the orders, which in turn will cause TWS to cancel the remaining orders. The investor may submit several 
    /// orders aimed at taking advantage of the most desirable price within the group. Completion of one piece of the group order causes 
    /// cancellation of the remaining group orders while partial completion causes the group to rebalance. An investor might desire to sell 
    /// 1000 shares of only ONE of three positions held above prevailing market prices. The OCA order group allows the investor to enter prices 
    /// at specified target levels and if one is completed, the other two will automatically cancel. Alternatively, an investor may wish to take 
    /// a LONG position in eMini S&P stock index futures in a falling market or else SELL US treasury futures at a more favorable price. 
    /// Grouping the two orders using an OCA order type offers the investor two chance to enter a similar position, while only running the risk 
    /// of taking on a single position.
    /// Products: BOND, CASH, FUT, FOP, STK, OPT, WAR
    /// </summary>
	// ! [oca]
void OrderSamples::OneCancelsAll(std::string ocaGroup, Order& ocaOrder, int ocaType){
		ocaOrder.ocaGroup = ocaGroup;
		ocaOrder.ocaType = ocaType;
}
	// ! [oca]

	/// <summary>
    /// Specific to US options, investors are able to create and enter Volatility-type orders for options and combinations rather than price orders. 
    /// Option traders may wish to trade and position for movements in the price of the option determined by its implied volatility. Because 
    /// implied volatility is a key determinant of the premium on an option, traders position in specific contract months in an effort to take 
    /// advantage of perceived changes in implied volatility arising before, during or after earnings or when company specific or broad market 
    /// volatility is predicted to change. In order to create a Volatility order, clients must first create a Volatility Trader page from the 
    /// Trading Tools menu and as they enter option contracts, premiums will display in percentage terms rather than premium. The buy/sell process 
    /// is the same as for regular orders priced in premium terms except that the client can limit the volatility level they are willing to pay or 
    /// receive.
    /// Products: FOP, OPT
    /// </summary>
Order OrderSamples::Volatility(std::string action, double quantity, double volatilityPercent, int volatilityType){
	// ! [volatility]
	Order order;
	order.action = action;
	order.orderType = "VOL";
	order.totalQuantity = quantity;
	order.volatility = volatilityPercent; //Expressed in percentage (40%)
	order.volatilityType = volatilityType; // 1=daily, 2=annual
	// ! [volatility]
	return order;
}

//! [fhedge]
Order OrderSamples::MarketFHedge(int parentOrderId, std::string action){
	//FX Hedge orders can only have a quantity of 0
	Order order = MarketOrder(action, 0);
	order.parentId = parentOrderId;
	order.hedgeType = "F";
	return order;
}
//! [fhedge]

Order OrderSamples::PeggedToBenchmark(std::string action, double quantity, double startingPrice, bool peggedChangeAmountDecrease, double peggedChangeAmount, double referenceChangeAmount, int referenceConId, std::string referenceExchange, double stockReferencePrice, double referenceContractLowerRange, double referenceContractUpperRange){
	//! [pegged_benchmark]
	Order order;
	order.orderType = "PEG BENCH";
	//BUY or SELL
	order.action = action;
	order.totalQuantity = quantity;
	//Beginning with price...
	order.startingPrice = startingPrice;
	//increase/decrease price..
	order.isPeggedChangeAmountDecrease = peggedChangeAmountDecrease;
	//by... (and likewise for price moving in opposite direction)
	order.peggedChangeAmount = peggedChangeAmount;
	//whenever there is a price change of...
	order.referenceChangeAmount = referenceChangeAmount;
	//in the reference contract...
	order.referenceContractId = referenceConId;
	//being traded at...
	order.referenceExchangeId = referenceExchange;
	//starting reference price is...
	order.stockRefPrice = stockReferencePrice;
	//Keep order active as long as reference contract trades between...
	order.stockRangeLower = referenceContractLowerRange;
	//and...
	order.stockRangeUpper = referenceContractUpperRange;
	//! [pegged_benchmark]
	return order;
}

Order OrderSamples::AttachAdjustableToStop(Order parent, double attachedOrderStopPrice, double triggerPrice, double adjustStopPrice){
	//! [adjustable_stop]
	//Attached order is a conventional STP order in opposite direction
	Order order;
	order.action = (parent.action == "BUY") ? "SELL": "BUY";
	order.orderType = "STP";
	order.totalQuantity = parent.totalQuantity;
	order.auxPrice = attachedOrderStopPrice;
	order.parentId = parent.orderId;
	//When trigger price is penetrated
	order.triggerPrice = triggerPrice;
	//The parent order will be turned into a STP order
	order.adjustedOrderType = "STP";
	//With the given STP price
	order.adjustedStopPrice = adjustStopPrice;
	//! [adjustable_stop]
	return order;
}

Order OrderSamples::AttachAdjustableToStopLimit(Order parent, double attachedOrderStopPrice, double triggerPrice, double adjustStopPrice, double adjustedStopLimitPrice){
	//! [adjustable_stop_limit]
	//Attached order is a conventional STP order
	Order order;
	order.action = (parent.action == "BUY") ? "SELL": "BUY";
	order.orderType = "STP";
	order.totalQuantity = parent.totalQuantity;
	order.auxPrice = attachedOrderStopPrice;
	order.parentId = parent.orderId;
	//When trigger price is penetrated
	order.triggerPrice = triggerPrice;
	//The parent order will be turned into a STP LMT order
	order.adjustedOrderType = "STP LMT";
	//With the given stop price
	order.adjustedStopPrice = adjustStopPrice;
	//And the given limit price
	order.adjustedStopLimitPrice = adjustedStopLimitPrice;
	//! [adjustable_stop_limit]
	return order;
}

OrderCondition* OrderSamples::Price_Condition(int conId, std::string exchange, double price, bool isMore, bool isConjunction){
	//! [price_condition]
	//Conditions have to be created via the OrderCondition.Create 
	PriceCondition* priceCondition = dynamic_cast<PriceCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::Price));
	//When this contract...
	priceCondition->conId(conId);
	//traded on this exchange
	priceCondition->exchange(exchange);
	//has a price above/below
	priceCondition->isMore(isMore);
	//this quantity
	priceCondition->price(price);
	//AND | OR next condition (will be ignored if no more conditions are added)
	priceCondition->conjunctionConnection(isConjunction);
	//! [price_condition]
	return dynamic_cast<OrderCondition *>(priceCondition);
}

OrderCondition* OrderSamples::Execution_Condition(std::string symbol, std::string secType, std::string exchange, bool isConjunction){
	//! [execution_condition]
	ExecutionCondition* execCondition = dynamic_cast<ExecutionCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::Execution));
	//When an execution on symbol
	execCondition->symbol(symbol);
	//at exchange
	execCondition->exchange(exchange);
	//for this secType
	execCondition->secType(secType);
	//AND | OR next condition (will be ignored if no more conditions are added)
	execCondition->conjunctionConnection(isConjunction);
	//! [execution_condition]
	return dynamic_cast<OrderCondition *>(execCondition);
}

OrderCondition* OrderSamples::Margin_Condition(int percent, bool isMore, bool isConjunction){
	//! [margin_condition]
	MarginCondition* marginCondition = dynamic_cast<MarginCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::Margin));
	//If margin is above/below
	marginCondition->percent(percent);
	//given percent
	marginCondition->isMore(isMore);
	//AND | OR next condition (will be ignored if no more conditions are added)
	marginCondition->conjunctionConnection(isConjunction);
	//! [margin_condition]
	return dynamic_cast<OrderCondition *>(marginCondition);
}

OrderCondition* OrderSamples::Percent_Change_Condition(double pctChange, int conId, std::string exchange, bool isMore, bool isConjunction){
	//! [percentage_condition]
	PercentChangeCondition* pctChangeCondition = dynamic_cast<PercentChangeCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::PercentChange));
	//If there is a price percent change measured against last close price above or below...
	pctChangeCondition->isMore(isMore);
	//this amount...
	pctChangeCondition->changePercent(pctChange);
	//on this contract
	pctChangeCondition->conId(conId);
	//when traded on this exchange...
	pctChangeCondition->exchange(exchange);
	//AND | OR next condition (will be ignored if no more conditions are added)
	pctChangeCondition->conjunctionConnection(isConjunction);
	//! [percentage_condition]
	return dynamic_cast<OrderCondition *>(pctChangeCondition);
}

OrderCondition* OrderSamples::Time_Condition(std::string time, bool isMore, bool isConjunction){
	//! [time_condition]
	TimeCondition* timeCondition = dynamic_cast<TimeCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::Time));
	//Before or after...
	timeCondition->isMore(isMore);
	//this time..
	timeCondition->time(time);
	//AND | OR next condition (will be ignored if no more conditions are added)  
	timeCondition->conjunctionConnection(isConjunction);
	//! [time_condition]
	return dynamic_cast<OrderCondition *>(timeCondition);
}

OrderCondition* OrderSamples::Volume_Condition(int conId, std::string exchange, bool isMore, int volume, bool isConjunction){
	//! [volume_condition]
	VolumeCondition* volCondition = dynamic_cast<VolumeCondition *>(OrderCondition::create(OrderCondition::OrderConditionType::Volume));
	//Whenever contract...
	volCondition->conId(conId);
	//When traded at
	volCondition->exchange(exchange);
	//reaches a volume higher/lower
	volCondition->isMore(isMore);
	//than this...
	volCondition->volume(volume);
	//AND | OR next condition (will be ignored if no more conditions are added)
	volCondition->conjunctionConnection(isConjunction);
	//! [volume_condition]
	return dynamic_cast<OrderCondition *>(volCondition);
}
