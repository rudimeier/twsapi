/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef HISTORICALTICKBIDASK_H
#define HISTORICALTICKBIDASK_H

struct HistoricalTickBidAsk
{
    long long time;
    int mask;
    double priceBid;
    double priceAsk;
    long long sizeBid;
    long long sizeAsk;
};

#endif /* HISTORICALTICKBIDASK_H */
