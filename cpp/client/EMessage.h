/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef EMESSAGE_H
#define EMESSAGE_H

class TWSAPIDLLEXP EMessage
{
    std::vector<char> data;
public:
    EMessage(const std::vector<char> &data);
    const char* begin(void) const;
    const char* end(void) const;
};

#endif /* EMESSAGE_H */
