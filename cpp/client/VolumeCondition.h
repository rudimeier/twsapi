/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#include "ContractCondition.h"

class TWSAPIDLLEXP VolumeCondition : public ContractCondition {
	friend OrderCondition;

	int m_volume;

protected:
	VolumeCondition() { }

	virtual std::string valueToString() const;
	virtual void valueFromString(const std::string &v);

public:
	static const OrderConditionType conditionType = OrderConditionType::Volume;

	int volume();
	void volume(int volume);
};