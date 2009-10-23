#include "StdAfx.h"

#include "EClientSocket.h"
#include "TwsSocketClientErrors.h"
#include "EWrapper.h"

#include <afxsock.h>

///////////////////////////////////////////////////////////
// MySocket definition

class EClientSocket::MySocket : public CAsyncSocket
{
public:
	MySocket( EClientSocket *pClient);
	void OnConnect( int i);
	void OnReceive( int i);
	void OnSend( int i);
	void OnClose( int i);
private:
	EClientSocket *m_pClient;
};


///////////////////////////////////////////////////////////
// MySocket implementation

EClientSocket::MySocket::MySocket( EClientSocket *pClient)
{
	m_pClient = pClient;
}

void EClientSocket::MySocket::OnConnect( int i)
{
	m_pClient->onConnect( i);
}

void EClientSocket::MySocket::OnReceive( int i)
{
	m_pClient->onReceive( i);
}

void EClientSocket::MySocket::OnSend( int i)
{
	m_pClient->onSend( i);
}

void EClientSocket::MySocket::OnClose( int i)
{
	m_pClient->onClose( i);
}

///////////////////////////////////////////////////////////
// member funcs
EClientSocket::EClientSocket( EWrapper *ptr) : EClientSocketBase( ptr)
{
}

EClientSocket::~EClientSocket()
{
	eDisconnect();
}

bool EClientSocket::eConnect( const char *host, UINT port, int clientId)
{
	// already connected?
	if( isConnected()) {
		getWrapper()->error( NO_VALID_ID, ALREADY_CONNECTED.code(), ALREADY_CONNECTED.msg());
		return false;
	}

	// init sockets
	AfxSocketInit();

	// close open connection if there was one
	eDisconnect();

	// create socket
	m_pSocket.reset(new MySocket(this));
	if( !m_pSocket->Create()) {
		eDisconnect();
		getWrapper()->winError( "Failed to create socket", GetLastError() );
		getWrapper()->error( NO_VALID_ID, FAIL_CREATE_SOCK.code(), FAIL_CREATE_SOCK.msg());
		return false;
	}

	// use local machine if no host passed in
	if( !(host && *host)) {
		host = "127.0.0.1";
	}

	// connect to server
	if( !m_pSocket->Connect(host, port)) {
		int lastError = GetLastError();
		if( lastError != WSAEWOULDBLOCK && !handleSocketError(GetLastError())) {
			return false;
		}
	}

	setClientId( clientId);

	{
		// Wait till we are fully connected (or for an error)
		CWinThread* pThread = AfxGetThread();
		while( m_pSocket.get() && !isConnected()) {
			if (!pThread->PumpMessage())
				return false;
		}
	}
	return true;
}

void EClientSocket::eDisconnect()
{
	eDisconnectBase();
	m_pSocket.reset();
}

int EClientSocket::send(const char* buf, size_t sz)
{
	if( sz <= 0)
		return 0;

	int nResult = m_pSocket->Send( buf, sz);
	if( nResult == SOCKET_ERROR && !handleSocketError( GetLastError())) {
		return -1;
	}
	if( nResult <= 0) {
		return 0;
	}
	return nResult;
}

int EClientSocket::receive(char* buf, size_t sz)
{
	if( sz <= 0)
		return 0;

	int nResult = m_pSocket->Receive( buf, sz);
	if( nResult == SOCKET_ERROR && !handleSocketError( GetLastError())) {
		return -1;
	}
	if( nResult <= 0) {
		return 0;
	}

	return nResult;
}

///////////////////////////////////////////////////////////
// callbacks from socket

void EClientSocket::onConnect( int i)
{
	if( !handleSocketError( i))
		return;

	onConnectBase();
}

void EClientSocket::onReceive( int i)
{
	if( !handleSocketError( i))
		return;

	checkMessages();
}

void EClientSocket::onSend( int i)
{
	if( !handleSocketError( i))
		return;

	sendBufferedData();
}

void EClientSocket::onClose( int i)
{
	// this function is called when the TWS terminates the connection

	eDisconnect();
	getWrapper()->connectionClosed();
}

///////////////////////////////////////////////////////////
// helper
bool EClientSocket::handleSocketError( int lastError)
{
	if( lastError == ERROR_SUCCESS)
		return true;

	if( lastError == WSAEWOULDBLOCK)
		return false;

	if( lastError == WSAECONNREFUSED) {
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
	}
	else {
		char lastErrorStr[512];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError, 0,
			lastErrorStr, sizeof(lastErrorStr), NULL);
		getWrapper()->error( NO_VALID_ID, SOCKET_EXCEPTION.code(),
			SOCKET_EXCEPTION.msg() + lastErrorStr);
	}
	eDisconnect();
	return false;
}

bool EClientSocket::isSocketOK() const
{
	return m_pSocket.get();
}
