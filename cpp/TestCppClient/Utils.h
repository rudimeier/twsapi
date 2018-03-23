/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */
#pragma once
#ifndef UTILS
#define UTILS

#include <vector>
#include <cstdint>

class Utils {

public:
    static inline bool is_base64(std::uint8_t c);
    static std::vector<std::uint8_t> base64_decode(std::string const&);
};

#endif
