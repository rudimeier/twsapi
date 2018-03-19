#pragma once
#ifndef ACCOUNTSUMMARYTAGS
#define ACCOUNTSUMMARYTAGS

#include <string>

class AccountSummaryTags{
public:
	static std::string AccountType;
    static std::string NetLiquidation;
    static std::string TotalCashValue;
    static std::string SettledCash;
    static std::string AccruedCash;
    static std::string BuyingPower;
    static std::string EquityWithLoanValue;
    static std::string PreviousEquityWithLoanValue;
    static std::string GrossPositionValue;
    static std::string ReqTEquity;
    static std::string ReqTMargin;
    static std::string SMA;
    static std::string InitMarginReq;
    static std::string MaintMarginReq;
    static std::string AvailableFunds;
    static std::string ExcessLiquidity;
    static std::string Cushion;
    static std::string FullInitMarginReq;
    static std::string FullMaintMarginReq;
    static std::string FullAvailableFunds;
    static std::string FullExcessLiquidity;
    static std::string LookAheadNextChange;
    static std::string LookAheadInitMarginReq;
    static std::string LookAheadMaintMarginReq;
    static std::string LookAheadAvailableFunds;
    static std::string LookAheadExcessLiquidity;
    static std::string HighestSeverity;
    static std::string DayTradesRemaining;
    static std::string Leverage;

	static std::string getAllTags();
};

#endif