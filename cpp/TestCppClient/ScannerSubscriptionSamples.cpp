/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#include "StdAfx.h"

#include "ScannerSubscription.h"
#include "ScannerSubscriptionSamples.h"

ScannerSubscription ScannerSubscriptionSamples::HotUSStkByVolume()
{
	//! [hotusvolume]
	//Hot US stocks by volume
	ScannerSubscription scanSub;
	scanSub.instrument = "STK";
	scanSub.locationCode = "STK.US.MAJOR";
	scanSub.scanCode = "HOT_BY_VOLUME";
	//! [hotusvolume]
	return scanSub;
}

ScannerSubscription ScannerSubscriptionSamples::TopPercentGainersIbis()
{
	//! [toppercentgaineribis]
	//Top % gainers at IBIS
	ScannerSubscription scanSub;
	scanSub.instrument = "STOCK.EU";
    scanSub.locationCode = "STK.EU.IBIS";
    scanSub.scanCode = "TOP_PERC_GAIN";
	//! [toppercentgaineribis]
	return scanSub;
}

ScannerSubscription ScannerSubscriptionSamples::MostActiveFutSoffex()
{
	//! [mostactivefutsoffex]
	//Most active futures at SOFFEX
	ScannerSubscription scanSub;
	scanSub.instrument = "FUT.EU";
    scanSub.locationCode = "FUT.EU.SOFFEX";
    scanSub.scanCode = "MOST_ACTIVE";
	//! [mostactivefutsoffex]
	return scanSub;
}

ScannerSubscription ScannerSubscriptionSamples::HighOptVolumePCRatioUSIndexes()
{
	//! [highoptvolume]
	//High option volume P/C ratio US indexes
	ScannerSubscription scanSub;
	scanSub.instrument = "IND.US";
    scanSub.locationCode = "IND.US";
    scanSub.scanCode = "HIGH_OPT_VOLUME_PUT_CALL_RATIO";
	//! [highoptvolume]
	return scanSub;
}