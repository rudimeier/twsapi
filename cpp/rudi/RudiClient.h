/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef eposixclientsocket_def
#define eposixclientsocket_def

#include "EClient.h"
#include "EClientMsgSink.h"
#include "ESocket.h"

class EWrapper;

class TWSAPIDLLEXP RudiClient : public EClient, public EClientMsgSink
{
protected:
    virtual void prepareBufferImpl(std::ostream&) const;
	virtual void prepareBuffer(std::ostream&) const;
	virtual bool closeAndSend(std::string msg, unsigned offset = 0);
	virtual int bufferedSend(const std::string& msg);

public:

	explicit RudiClient(EWrapper *ptr);
	virtual ~RudiClient();

	bool eConnect( const char *host, unsigned int port, int clientId = 0, bool extraAuth = false);
	// override virtual funcs from EClient
	void eDisconnect();
	bool eConnect2( const char *host, unsigned int port, int clientId=0,
		int family=0, bool extraAuth = false );

	bool isSocketOK() const;
	int fd() const;
    bool asyncEConnect() const;
    void asyncEConnect(bool val);
    ESocket *getTransport();

private:
	void encodeMsgLen(std::string& msg, unsigned offset) const;
public:
	int receive( char* buf, size_t sz);

public:
	// callback from socket
	void onSend();
	void onError();
	void onClose();

private:

	int m_fd;
    bool m_allowRedirect;
    const char* m_hostNorm;
    bool m_asyncEConnect;

//EClientMsgSink implementation
public:
    void serverVersion(int version, const char *time);
    void redirect(const char *host, unsigned int port);    
};

#endif
