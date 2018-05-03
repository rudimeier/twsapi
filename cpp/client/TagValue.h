/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef tagvalue_def
#define tagvalue_def


#include <string>
#include <vector>
#include <memory>

struct TagValue
{
	TagValue() {}
	TagValue(const std::string& p_tag, const std::string& p_value)
		: tag(p_tag), value(p_value)
	{}

	std::string tag;
	std::string value;
};

typedef std::shared_ptr<TagValue> TagValueSPtr;
typedef std::vector<TagValueSPtr> TagValueList;
typedef std::shared_ptr<TagValueList> TagValueListSPtr;

#endif

