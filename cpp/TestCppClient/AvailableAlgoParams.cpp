/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include "StdAfx.h"

#include "AvailableAlgoParams.h"
#include "Order.h"

//! [arrivalpx_params]
void AvailableAlgoParams::FillArrivalPriceParams(Order& baseOrder, double maxPctVol, std::string riskAversion, std::string startTime, std::string endTime, 
	bool forceCompletion, bool allowPastTime, double monetaryValue){
	baseOrder.algoStrategy = "ArrivalPx";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("maxPctVol", std::to_string(maxPctVol)));
	TagValueSPtr tag2(new TagValue("riskAversion", riskAversion));
	TagValueSPtr tag3(new TagValue("startTime", startTime));
	TagValueSPtr tag4(new TagValue("endTime", endTime));
	TagValueSPtr tag5(new TagValue("forceCompletion", forceCompletion ? "1" : "0"));
	TagValueSPtr tag6(new TagValue("allowPastEndTime", allowPastTime ? "1" : "0"));
	TagValueSPtr tag7(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
}
//! [arrivalpx_params]

//! [darkice_params]
void AvailableAlgoParams::FillDarkIceParams(Order& baseOrder, int displaySize, std::string startTime, std::string endTime, bool allowPastEndTime, double monetaryValue){
	baseOrder.algoStrategy = "DarkIce";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("displaySize", std::to_string(displaySize)));
	TagValueSPtr tag2(new TagValue("startTime", startTime));
	TagValueSPtr tag3(new TagValue("endTime", endTime));
	TagValueSPtr tag4(new TagValue("allowPastEndTime", allowPastEndTime ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
}
//! [darkice_params]

//! [pctvol_params]
void AvailableAlgoParams::FillPctVolParams(Order& baseOrder, double pctVol, std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue){
	baseOrder.algoStrategy = "PctVol";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("pctVol", std::to_string(pctVol)));
	TagValueSPtr tag2(new TagValue("startTime", startTime));
	TagValueSPtr tag3(new TagValue("endTime", endTime));
	TagValueSPtr tag4(new TagValue("noTakeLiq", noTakeLiq ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
}
//! [pctvol_params]

//! [twap_params]
void AvailableAlgoParams::FillTwapParams(Order& baseOrder, std::string strategyType, std::string startTime, std::string endTime, bool allowPastEndTime, double monetaryValue){
	baseOrder.algoStrategy = "Twap";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("strategyType", strategyType));
	TagValueSPtr tag2(new TagValue("startTime", startTime));
	TagValueSPtr tag3(new TagValue("endTime", endTime));
	TagValueSPtr tag4(new TagValue("allowPastEndTime", allowPastEndTime ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
}
//! [twap_params]

//! [vwap_params]
void AvailableAlgoParams::FillVwapParams(Order& baseOrder, double maxPctVol, std::string startTime, std::string endTime, bool allowPastEndTime, bool noTakeLiq, bool speedUp, double monetaryValue){
	baseOrder.algoStrategy = "Vwap";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("maxPctVol", std::to_string(maxPctVol)));
	TagValueSPtr tag2(new TagValue("startTime", startTime));
	TagValueSPtr tag3(new TagValue("endTime", endTime));
	TagValueSPtr tag4(new TagValue("allowPastEndTime", allowPastEndTime ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("noTakeLiq", noTakeLiq ? "1" : "0"));
	TagValueSPtr tag6(new TagValue("speedUp", speedUp ? "1" : "0"));
	TagValueSPtr tag7(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
}
//! [vwap_params]

//! [ad_params]
void AvailableAlgoParams::FillAccumulateDistributeParams(Order& baseOrder, int componentSize, int timeBetweenOrders, bool randomizeTime20, bool randomizeSize55,
	int giveUp, bool catchUp, bool waitForFill, std::string startTime, std::string endTime){
	baseOrder.algoStrategy = "AD";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("componentSize", std::to_string(componentSize)));
	TagValueSPtr tag2(new TagValue("timeBetweenOrders",std::to_string(timeBetweenOrders)));
	TagValueSPtr tag3(new TagValue("randomizeTime20", randomizeTime20 ? "1" : "0"));
	TagValueSPtr tag4(new TagValue("randomizeSize55", randomizeSize55 ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("giveUp", std::to_string(giveUp)));
	TagValueSPtr tag6(new TagValue("catchUp", catchUp ? "1" : "0"));
	TagValueSPtr tag7(new TagValue("waitForFill", waitForFill ? "1" : "0"));
	TagValueSPtr tag8(new TagValue("startTime", startTime));
	TagValueSPtr tag9(new TagValue("endTime", endTime));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
	baseOrder.algoParams->push_back(tag8);
	baseOrder.algoParams->push_back(tag9);
}
//! [ad_params]

//! [balanceimpactrisk_params]
void AvailableAlgoParams::FillBalanceImpactRiskParams(Order& baseOrder, double maxPctVol, std::string riskAversion, bool forceCompletion){
	baseOrder.algoStrategy = "BalanceImpactRisk";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("maxPctVol", std::to_string(maxPctVol)));
	TagValueSPtr tag2(new TagValue("riskAversion", riskAversion));
	TagValueSPtr tag3(new TagValue("forceCompletion", forceCompletion ? "1" : "0"));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
}
//! [balanceimpactrisk_params]

//! [minimpact_params]
void AvailableAlgoParams::FillMinImpactParams(Order& baseOrder, double maxPctVol){
	baseOrder.algoStrategy = "MinImpact";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("maxPctVol", std::to_string(maxPctVol)));
	baseOrder.algoParams->push_back(tag1);
}
//! [minimpact_params]

//! [adaptive_params]
void AvailableAlgoParams::FillAdaptiveParams(Order& baseOrder, std::string priority){
	baseOrder.algoStrategy = "Adaptive";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("adaptivePriority", priority));
	baseOrder.algoParams->push_back(tag1);
}
//! [adaptive_params]

//! [closepx_params]
void AvailableAlgoParams::FillClosePriceParams(Order& baseOrder, double maxPctVol, std::string riskAversion, std::string startTime, bool forceCompletion, double monetaryValue){
	baseOrder.algoStrategy = "ClosePx";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("maxPctVol", std::to_string(maxPctVol)));
	TagValueSPtr tag2(new TagValue("riskAversion",riskAversion));
	TagValueSPtr tag3(new TagValue("startTime", startTime));
	TagValueSPtr tag4(new TagValue("forceCompletion", forceCompletion ? "1" : "0"));
	TagValueSPtr tag5(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
}
//! [closepx_params]

//! [pctvolpx_params]
void AvailableAlgoParams::FillPriceVariantPctVolParams(Order baseOrder, double pctVol, double deltaPctVol, double minPctVol4Px, 
													   double maxPctVol4Px, std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue){
	baseOrder.algoStrategy = "PctVolPx";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("pctVol", std::to_string(pctVol)));
	TagValueSPtr tag2(new TagValue("deltaPctVol", std::to_string(deltaPctVol)));
	TagValueSPtr tag3(new TagValue("minPctVol4Px", std::to_string(minPctVol4Px)));
	TagValueSPtr tag4(new TagValue("maxPctVol4Px", std::to_string(maxPctVol4Px)));
	TagValueSPtr tag5(new TagValue("startTime", startTime));
	TagValueSPtr tag6(new TagValue("endTime", endTime));
	TagValueSPtr tag7(new TagValue("noTakeLiq", noTakeLiq ? "1" : "0"));
	TagValueSPtr tag8(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
	baseOrder.algoParams->push_back(tag8);
}
//! [pctvolpx_params]

//! [pctvolsz_params]
void AvailableAlgoParams::FillSizeVariantPctVolParams(Order baseOrder, double startPctVol, double endPctVol, 
								std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue){
	baseOrder.algoStrategy = "PctVolSz";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("startPctVol", std::to_string(startPctVol)));
	TagValueSPtr tag2(new TagValue("endPctVol", std::to_string(endPctVol)));
	TagValueSPtr tag3(new TagValue("startTime", startTime));
	TagValueSPtr tag4(new TagValue("endTime", endTime));
	TagValueSPtr tag5(new TagValue("noTakeLiq", noTakeLiq ? "1" : "0"));
	TagValueSPtr tag6(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
}
//! [pctvolsz_params]

//! [pctvoltm_params]
void AvailableAlgoParams::FillTimeVariantPctVolParams(Order baseOrder, double startPctVol, double endPctVol, std::string startTime, 
								 std::string endTime, bool noTakeLiq, double monetaryValue){
	baseOrder.algoStrategy = "PctVolTm";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("startPctVol", std::to_string(startPctVol)));
	TagValueSPtr tag2(new TagValue("endPctVol", std::to_string(endPctVol)));
	TagValueSPtr tag3(new TagValue("startTime", startTime));
	TagValueSPtr tag4(new TagValue("endTime", endTime));
	TagValueSPtr tag5(new TagValue("noTakeLiq", noTakeLiq ? "1" : "0"));
	TagValueSPtr tag6(new TagValue("monetaryValue", std::to_string(monetaryValue)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
}
//! [pctvoltm_params]

//! [jefferies_vwap_params]
void AvailableAlgoParams::FillJefferiesVWAPParams(Order baseOrder, std::string startTime, std::string endTime, double relativeLimit, 
		double maxVolumeRate, std::string excludeAuctions, double triggerPrice, double wowPrice, int minFillSize, double wowOrderPct, 
		std::string wowMode, bool isBuyBack, std::string wowReference){
	baseOrder.algoStrategy = "VWAP";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("StartTime", startTime));
	TagValueSPtr tag2(new TagValue("EndTime", endTime));
	TagValueSPtr tag3(new TagValue("RelativeLimit", std::to_string(relativeLimit)));
	TagValueSPtr tag4(new TagValue("MaxVolumeRate", std::to_string(maxVolumeRate)));
	TagValueSPtr tag5(new TagValue("ExcludeAuctions", excludeAuctions));
	TagValueSPtr tag6(new TagValue("TriggerPrice", std::to_string(triggerPrice)));
	TagValueSPtr tag7(new TagValue("WowPrice", std::to_string(wowPrice)));
	TagValueSPtr tag8(new TagValue("MinFillSize", std::to_string(minFillSize)));
	TagValueSPtr tag9(new TagValue("WowOrderPct", std::to_string(wowOrderPct)));
	TagValueSPtr tag10(new TagValue("WowMode", wowMode));
	TagValueSPtr tag11(new TagValue("IsBuyBack", isBuyBack ? "1" : "0"));
	TagValueSPtr tag12(new TagValue("WowReference", wowReference));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
	baseOrder.algoParams->push_back(tag8);
	baseOrder.algoParams->push_back(tag9);
	baseOrder.algoParams->push_back(tag10);
	baseOrder.algoParams->push_back(tag11);
	baseOrder.algoParams->push_back(tag12);
}
//! [jefferies_vwap_params]

//! [csfb_inline_params]
void AvailableAlgoParams::FillCSFBInlineParams(Order baseOrder, std::string startTime, std::string endTime, std::string execStyle, int minPercent,
		int maxPercent, int displaySize, std::string auction, bool blockFinder, double blockPrice, int minBlockSize, int maxBlockSize, double iWouldPrice){
	
	// must be direct-routed to "CSFBALGO"

	baseOrder.algoStrategy = "INLINE";
	baseOrder.algoParams.reset(new TagValueList());
	TagValueSPtr tag1(new TagValue("StartTime", startTime));
	TagValueSPtr tag2(new TagValue("EndTime", endTime));
	TagValueSPtr tag3(new TagValue("ExecStyle", execStyle));
	TagValueSPtr tag4(new TagValue("MinPercent", std::to_string(minPercent)));
	TagValueSPtr tag5(new TagValue("MaxPercent", std::to_string(maxPercent)));
	TagValueSPtr tag6(new TagValue("DisplaySize", std::to_string(displaySize)));
	TagValueSPtr tag7(new TagValue("Auction", auction));
	TagValueSPtr tag8(new TagValue("BlockFinder", blockFinder ? "1" : "0"));
	TagValueSPtr tag9(new TagValue("BlockPrice", std::to_string(blockPrice)));
	TagValueSPtr tag10(new TagValue("MinBlockSize", std::to_string(minBlockSize)));
	TagValueSPtr tag11(new TagValue("MaxBlockSize", std::to_string(maxBlockSize)));
	TagValueSPtr tag12(new TagValue("IWouldPrice", std::to_string(iWouldPrice)));
	baseOrder.algoParams->push_back(tag1);
	baseOrder.algoParams->push_back(tag2);
	baseOrder.algoParams->push_back(tag3);
	baseOrder.algoParams->push_back(tag4);
	baseOrder.algoParams->push_back(tag5);
	baseOrder.algoParams->push_back(tag6);
	baseOrder.algoParams->push_back(tag7);
	baseOrder.algoParams->push_back(tag8);
	baseOrder.algoParams->push_back(tag9);
	baseOrder.algoParams->push_back(tag10);
	baseOrder.algoParams->push_back(tag11);
	baseOrder.algoParams->push_back(tag12);
}
//! [csfb_inline_params]