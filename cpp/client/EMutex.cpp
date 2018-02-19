/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"
#include "EMutex.h"

EMutex::EMutex() {
#if defined(IB_POSIX)
#elif defined(IB_WIN32)
    InitializeCriticalSection(&cs);
#else
#   error "Not implemented on this platform"
#endif
}

EMutex::~EMutex(void) {
#if defined(IB_POSIX)
#elif defined(IB_WIN32)
    DeleteCriticalSection(&cs);
#else
#   error "Not implemented on this platform"
#endif
}

bool EMutex::TryEnter() {
#if defined(IB_POSIX)
    return cs.try_lock();
#elif defined(IB_WIN32)
    return TryEnterCriticalSection(&cs);
#else
#   error "Not implemented on this platform"
#endif
}

void EMutex::Enter() {
#if defined(IB_POSIX)
    cs.lock();  
#elif defined(IB_WIN32)
    EnterCriticalSection(&cs);
#else
#   error "Not implemented on this platform"
#endif
}

void EMutex::Leave() {
#if defined(IB_POSIX)
    cs.unlock();  
#elif defined(IB_WIN32)
    LeaveCriticalSection(&cs);
#else
#   error "Not implemented on this platform"
#endif
}


EMutexGuard::EMutexGuard(EMutex& m) : m_mutex(m) {
    m_mutex.Enter();
}

EMutexGuard::~EMutexGuard() {
    m_mutex.Leave();
}

