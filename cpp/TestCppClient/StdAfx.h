/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#include "client/StdAfx.h"

#ifndef TWSAPIDLL
#ifndef TWSAPIDLLEXP
#define TWSAPIDLLEXP __declspec(dllimport)
#endif
#endif
#endif /* STDAFX_H */
