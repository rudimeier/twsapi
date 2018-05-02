/* Copyright (C) 2018 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef testcppclient_h__INCLUDED
#define testcppclient_h__INCLUDED

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "../ssl/EReaderSSL.h"

#include <memory>

class EClientSocketSSL;

enum State {
	ST_CONNECT,
	ST_PLACEORDER,
	ST_PLACEORDER_ACK,
	ST_CANCELORDER,
	ST_CANCELORDER_ACK,
	ST_PING,
	ST_PING_ACK,
	ST_IDLE
};


class TestCppClient : public EWrapper
{
public:

	TestCppClient();
	~TestCppClient();

	void setConnectOptions(const std::string&);
	void processMessages();

public:

	bool connect(const char * host, unsigned int port, int clientId = 0);
	void disconnect() const;
	bool isConnected() const;

private:

	void reqCurrentTime();
	void placeOrder();
	void cancelOrder();

public:
	// events
	#include "EWrapper_prototypes.h"

private:
    EReaderOSSignal m_osSignal;
    EClientSocketSSL * const m_pClient;
	State m_state;
	time_t m_sleepDeadline;

	OrderId m_orderId;
	EReaderSSL *m_pReader;
	
    bool m_extraAuth;
};

#endif

