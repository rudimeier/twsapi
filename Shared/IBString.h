/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef ibstring_h__INCLUDED
#define ibstring_h__INCLUDED

#include <string>
typedef std::string IBString;

#include <stdlib.h>

inline bool IsEmpty(const IBString& str)
{
	return str.empty();
};

inline void Empty(IBString& str)
{
	str.erase();
};

inline bool Compare(IBString str, const char* strToCompare)
{
	return str.compare(strToCompare);
};

inline bool Compare(IBString str, IBString strToCompare)
{
	return str.compare(strToCompare);
};

inline double Atof(IBString str)
{
	return atof(str.c_str());
};

inline int Atoi(IBString str)
{
	return atoi(str.c_str());
};

#endif


