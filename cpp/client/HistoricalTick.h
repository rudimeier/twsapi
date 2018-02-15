/* Copyright (C) 2017 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef historicaltickendtry_def
#define historicaltickendtry_def

struct HistoricalTick
{
    long long time;
    double price;
    long long size;
};  
#endif // !historicaltickendtry_def


