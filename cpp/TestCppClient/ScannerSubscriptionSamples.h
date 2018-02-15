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