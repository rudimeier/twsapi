/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#include "ContractCondition.h"
#include "Order.h"

class TWSAPIDLLEXP PercentChangeCondition : public ContractCondition {
	friend OrderCondition;

	double m_changePercent;

protected:
	PercentChangeCondition()
	: m_changePercent(UNSET_DOUBLE)
	{ }

	virtual std::string valueToString() const;
	virtual void valueFromString(const std::string &v);

public:
	static const OrderConditionType conditionType = OrderConditionType::PercentChange;

	double changePercent();
	void changePercent(double changePercent);
};