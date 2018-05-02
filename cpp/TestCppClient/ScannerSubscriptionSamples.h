/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef SCANNERSUBSCRIPTIONSAMPLES
#define SCANNERSUBSCRIPTIONSAMPLES

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