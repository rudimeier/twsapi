/* Copyright (C) 2017 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef depthmktdatadescription_def
#define depthmktdatadescription_def

#include <limits.h>
#define UNSET_INTEGER INT_MAX

struct DepthMktDataDescription
{
	DepthMktDataDescription()
		: exchange("")
		, secType("")
		, listingExch("")
		, serviceDataType("")
		, aggGroup(UNSET_INTEGER)
	{
	}

	std::string exchange;
	std::string secType;
	std::string listingExch;
	std::string serviceDataType;
	int aggGroup;
};

#endif // depthmktdatadescription_def
