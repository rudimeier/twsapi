#include "StdAfx.h"

#include "AccountSummaryTags.h"

std::string AccountSummaryTags::AccountType = "AccountType";
std::string AccountSummaryTags::NetLiquidation = "NetLiquidation";
std::string AccountSummaryTags::TotalCashValue = "TotalCashValue";
std::string AccountSummaryTags::SettledCash = "SettledCash";
std::string AccountSummaryTags::AccruedCash = "AccruedCash";
std::string AccountSummaryTags::BuyingPower = "BuyingPower";
std::string AccountSummaryTags::EquityWithLoanValue = "EquityWithLoanValue";
std::string AccountSummaryTags::PreviousEquityWithLoanValue = "PreviousEquityWithLoanValue";
std::string AccountSummaryTags::GrossPositionValue = "GrossPositionValue";
std::string AccountSummaryTags::ReqTEquity = "ReqTEquity";
std::string AccountSummaryTags::ReqTMargin = "ReqTMargin";
std::string AccountSummaryTags::SMA = "SMA";
std::string AccountSummaryTags::InitMarginReq = "InitMarginReq";
std::string AccountSummaryTags::MaintMarginReq = "MaintMarginReq";
std::string AccountSummaryTags::AvailableFunds = "AvailableFunds";
std::string AccountSummaryTags::ExcessLiquidity = "ExcessLiquidity";
std::string AccountSummaryTags::Cushion = "Cushion";
std::string AccountSummaryTags::FullInitMarginReq = "FullInitMarginReq";
std::string AccountSummaryTags::FullMaintMarginReq = "FullMaintMarginReq";
std::string AccountSummaryTags::FullAvailableFunds = "FullAvailableFunds";
std::string AccountSummaryTags::FullExcessLiquidity = "FullExcessLiquidity";
std::string AccountSummaryTags::LookAheadNextChange = "LookAheadNextChange";
std::string AccountSummaryTags::LookAheadInitMarginReq = "LookAheadInitMarginReq";
std::string AccountSummaryTags::LookAheadMaintMarginReq = "LookAheadMaintMarginReq";
std::string AccountSummaryTags::LookAheadAvailableFunds = "LookAheadAvailableFunds";
std::string AccountSummaryTags::LookAheadExcessLiquidity = "LookAheadExcessLiquidity";
std::string AccountSummaryTags::HighestSeverity = "HighestSeverity";
std::string AccountSummaryTags::DayTradesRemaining = "DayTradesRemaining";
std::string AccountSummaryTags::Leverage = "Leverage";

std::string AccountSummaryTags::getAllTags(){
	return AccountType + "," + NetLiquidation + "," + TotalCashValue + "," + SettledCash + "," + AccruedCash + "," + BuyingPower + "," + EquityWithLoanValue + "," + PreviousEquityWithLoanValue + "," + GrossPositionValue + "," + ReqTEquity
                + "," + ReqTMargin + "," + SMA + "," + InitMarginReq + "," + MaintMarginReq + "," + AvailableFunds + "," + ExcessLiquidity + "," + Cushion + "," + FullInitMarginReq + "," + FullMaintMarginReq + "," + FullAvailableFunds + "," + FullExcessLiquidity
                + "," + LookAheadNextChange + "," + LookAheadInitMarginReq + "," + LookAheadMaintMarginReq + "," + LookAheadAvailableFunds + "," + LookAheadExcessLiquidity + "," + HighestSeverity + "," + DayTradesRemaining + "," + Leverage;
}