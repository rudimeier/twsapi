/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef TAGVALUE_H
#define TAGVALUE_H

#include "shared_ptr.h"

#include <string>
#include <vector>

struct TagValue
{
	TagValue() {}
	TagValue(const std::string& p_tag, const std::string& p_value)
		: tag(p_tag), value(p_value)
	{}

	std::string tag;
	std::string value;
};

typedef ibapi::shared_ptr<TagValue> TagValueSPtr;
typedef std::vector<TagValueSPtr> TagValueList;
typedef ibapi::shared_ptr<TagValueList> TagValueListSPtr;

#endif /* TAGVALUE_H */
