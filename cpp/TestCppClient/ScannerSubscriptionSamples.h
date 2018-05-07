/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TWS_API_SAMPLES_TESTCPPCLIENT_SCANNERSUBSCRIPTIONSAMPLES_H
#define TWS_API_SAMPLES_TESTCPPCLIENT_SCANNERSUBSCRIPTIONSAMPLES_H

#include <string>

struct ScannerSubscription;

class ScannerSubscriptionSamples {
public:
	static ScannerSubscription HotUSStkByVolume();
	static ScannerSubscription TopPercentGainersIbis();
	static ScannerSubscription MostActiveFutSoffex();
	static ScannerSubscription HighOptVolumePCRatioUSIndexes();
};

#endif