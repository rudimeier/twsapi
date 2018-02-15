#pragma once
#ifndef AVAILABLEALGOPARAMS
#define AVAILABLEALGOPARAMS

#include <string>

struct Order;

class AvailableAlgoParams {
public:
	static void FillArrivalPriceParams(Order& baseOrder, double maxPctVol, std::string riskAversion, std::string startTime, std::string endTime, 
            bool forceCompletion, bool allowPastTime, double monetaryValue);
	static void FillDarkIceParams(Order& baseOrder, int displaySize, std::string startTime, std::string endTime, bool allowPastEndTime, double monetaryValue);
	static void FillPctVolParams(Order& baseOrder, double pctVol, std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue);
	static void FillTwapParams(Order& baseOrder, std::string strategyType, std::string startTime, std::string endTime, bool allowPastEndTime, double monetaryValue);
	static void FillVwapParams(Order& baseOrder, double maxPctVol, std::string startTime, std::string endTime, bool allowPastEndTime, bool noTakeLiq, bool speedUp, double monetaryValue);
	static void FillAccumulateDistributeParams(Order& baseOrder, int componentSize, int timeBetweenOrders, bool randomizeTime20, bool randomizeSize55,
            int giveUp, bool catchUp, bool waitForFill, std::string startTime, std::string endTime);
	static void FillBalanceImpactRiskParams(Order& baseOrder, double maxPctVol, std::string riskAversion, bool forceCompletion);
	static void FillMinImpactParams(Order& baseOrder, double maxPctVol);
	static void FillAdaptiveParams(Order& baseOrder, std::string priority);
	static void FillClosePriceParams(Order& baseOrder, double maxPctVol, std::string riskAversion, std::string startTime, bool forceCompletion, double monetaryValue);
	static void FillPriceVariantPctVolParams(Order baseOrder, double pctVol, double deltaPctVol, double minPctVol4Px, 
			double maxPctVol4Px, std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue);
	static void FillSizeVariantPctVolParams(Order baseOrder, double startPctVol, double endPctVol, 
			std::string startTime, std::string endTime, bool noTakeLiq, double monetaryValue);
	static void FillTimeVariantPctVolParams(Order baseOrder, double startPctVol, double endPctVol, std::string startTime, 
			std::string endTime, bool noTakeLiq, double monetaryValue);
	static void FillJefferiesVWAPParams(Order baseOrder, std::string startTime, std::string endTime, double relativeLimit, 
		double maxVolumeRate, std::string excludeAuctions, double triggerPrice, double wowPrice, int minFillSize, double wowOrderPct, 
		std::string wowMode, bool isBuyBack, std::string wowReference);
	static void FillCSFBInlineParams(Order baseOrder, std::string startTime, std::string endTime, std::string execStyle, int minPercent,
		int maxPercent, int displaySize, std::string auction, bool blockFinder, double blockPrice, int minBlockSize, int maxBlockSize, double iWouldPrice);
};

#endif
