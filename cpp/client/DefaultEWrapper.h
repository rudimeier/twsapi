/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#include "EWrapper.h"

class TWSAPIDLLEXP DefaultEWrapper :
    public EWrapper
{
public:
	#include "EWrapper_prototypes.h"
};
