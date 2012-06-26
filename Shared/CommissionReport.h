#ifndef commissionreport_def
#define commissionreport_def

#include "IBString.h"

namespace IB {

struct CommissionReport
{
	CommissionReport()
	{
		commission = 0;
		realizedPNL = 0;
		yield = 0;
		yieldRedemptionDate = 0;
	}

	// commission report fields
	IBString	execId;
	double		commission;
	IBString	currency;
	double		realizedPNL;
	double		yield;
	int			yieldRedemptionDate; // YYYYMMDD format
};

} // namespace IB
#endif // commissionreport_def
