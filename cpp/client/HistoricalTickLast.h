/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef historicalticklastendtry_def
#define historicalticklastendtry_def

struct HistoricalTickLast
{
    long long time;
    int mask; 
    double price;
    long long size;
    std::string exchange;
    std::string specialConditions;
};
#endif

