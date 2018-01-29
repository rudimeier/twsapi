#pragma once
#ifndef AVAILABLEALGOPARAMS
#define AVAILABLEALGOPARAMS

#include <string>

class Order;

class AvailableAlgoParams {
public:
	static void FillArrivalPriceParams(Order& baseOrder, double maxPctVol, std::string riskAversion, std::string startTime, std::string endTime, 
            bool forceCompletion, bool allowPastTime);
	static void FillDarkIceParams(Order& baseOrder, int displaySize, std::string startTime, std::string endTime, bool allowPastEndTime);
	static void FillPctVolParams(Order& baseOrder, double pctVol, std::string startTime, std::string endTime, bool noTakeLiq);
	static void FillTwapParams(Order& baseOrder, std::string strategyType, std::string startTime, std::string endTime, bool allowPastEndTime);
	static void FillVwapParams(Order& baseOrder, double maxPctVol, std::string startTime, std::string endTime, bool allowPastEndTime, bool noTakeLiq);
	static void FillAccumulateDistributeParams(Order& baseOrder, int componentSize, int timeBetweenOrders, bool randomizeTime20, bool randomizeSize55,
            int giveUp, bool catchUp, bool waitOrFill, std::string startTime, std::string endTime);
	static void FillBalanceImpactRiskParams(Order& baseOrder, double maxPctVol, std::string riskAversion, bool forceCompletion);
	static void FillMinImpactParams(Order& baseOrder, double maxPctVol);
	static void FillAdaptiveParams(Order& baseOrder, std::string priority);
};

#endif