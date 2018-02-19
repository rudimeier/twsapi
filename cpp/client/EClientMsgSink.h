/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef ECLIENTMSGSINK_H
#define ECLIENTMSGSINK_H

struct EClientMsgSink
{
    virtual void serverVersion(int version, const char *time) = 0;
    virtual void redirect(const char *host, unsigned int port) = 0;
    virtual ~EClientMsgSink() {}
};

#endif /* ECLIENTMSGSINK_H */
