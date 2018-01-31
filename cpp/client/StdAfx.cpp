/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"
#include <thread>

std::thread::id main_thread_id = std::this_thread::get_id();

const char* thread_str()
{
	if ( main_thread_id == std::this_thread::get_id() )
		return "main";
	else
		return "rder";
}
