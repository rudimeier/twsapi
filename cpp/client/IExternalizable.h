/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#include <ios>

struct IExternalizable
{
	virtual const char* readExternal(const char* ptr, const char* endPtr) = 0;
	virtual void writeExternal(std::ostream &out) const = 0;
};

