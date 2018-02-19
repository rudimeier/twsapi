/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef SSL_PRIVATE_H
#define SSL_PRIVATE_H

#include <openssl/ssl.h>
#include <openssl/err.h>

/* structs for SSL forward declaration and implicit cast */

struct SSL_P : SSL
{
	operator SSL*() {
		return (SSL*)this;
	}
};

struct SSL_CTX_P : SSL_CTX
{
	operator SSL_CTX*() {
		return (SSL_CTX*)this;
	}
};

#endif /* SSL_PRIVATE_H */
