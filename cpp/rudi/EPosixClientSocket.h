/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef eposixclientsocket_def
#define eposixclientsocket_def

#include "EClient.h"
#include "EClientMsgSink.h"
#include "ESocket.h"

class EWrapper;
class RudiReader;

class TWSAPIDLLEXP EPosixClientSocket : public EClient, public EClientMsgSink
{
protected:
    virtual void prepareBufferImpl(std::ostream&) const;
	virtual void prepareBuffer(std::ostream&) const;
	virtual bool closeAndSend(std::string msg, unsigned offset = 0);
	virtual int bufferedSend(const std::string& msg);

public:

	explicit EPosixClientSocket(EWrapper *ptr);
	virtual ~EPosixClientSocket();

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
	void on_send_errno(int xerrno);

public:
	int receive( char* buf, size_t sz);

public:
	// callback from socket
	void onSend();
	void onReceive();
	void onClose();
	void select_timeout(int msec);

private:

	int m_fd;
    bool m_allowRedirect;
    const char* m_hostNorm;
    bool m_asyncEConnect;
	/* This stupid m_in_connect indicates that we are inside eConnect(). We'd
	 * like to use m_connState instead but it's badly maintained by EClient. */
	bool m_in_connect;
	RudiReader *m_reader;

//EClientMsgSink implementation
public:
    void serverVersion(int version, const char *time);
    void redirect(const char *host, unsigned int port);    
};

#endif
