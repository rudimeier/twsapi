/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifdef _MSC_VER

#ifdef TWSAPIDLL
#ifndef TWSAPIDLLEXP
#define TWSAPIDLLEXP __declspec(dllexport)
#endif
#endif

#define assert ASSERT
#if _MSC_VER<=1800
#define snprintf _snprintf
#endif
#include <WinSock2.h>
#include <Windows.h>
#define IB_WIN32
#define atoll _atoi64
#else

#include <unistd.h> // defines _POSIX_THREADS, @see http://bit.ly/1pWJ8KQ#tag_13_80_03_02

#if defined(_POSIX_THREADS) && (_POSIX_THREADS > 0)
    #include <pthread.h>
    #define IB_POSIX
    #if __cplusplus >= 201103L // strict C++11 standard std::mutex is available
        #define IBAPI_STD_MUTEX
    #endif 
#else
    #error "Not supported on this platform"
#endif

#endif // #ifdef _MSC_VER

#include <string>
#include <deque>
#include <vector>
#include <algorithm>

#ifndef TWSAPIDLLEXP
#define TWSAPIDLLEXP
#endif

