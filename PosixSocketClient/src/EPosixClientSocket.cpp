#include "EPosixClientSocket.h"

#include "EPosixClientSocketPlatform.h"
#include "TwsSocketClientErrors.h"
#include "EWrapper.h"

#include <string.h>

namespace IB {

///////////////////////////////////////////////////////////
// static helper
bool resolveHost( const char *host, sockaddr_in *sa )
{
	if (sa->sin_addr.s_addr != INADDR_NONE) {
		/* No need to resolve it. */
		return true;
	}
	
	struct hostent hostbuf, *hp;
	size_t hstbuflen;
	char *tmphstbuf;
	int res;
	int herr;

	hstbuflen = 1024;
	/* Allocate buffer, remember to free it.  */
	tmphstbuf = (char*) malloc (hstbuflen);
	
	while( (res = gethostbyname_r (host, &hostbuf, tmphstbuf, hstbuflen,
		&hp, &herr)) == ERANGE ) {
		/* Enlarge the buffer.  */
		hstbuflen *= 2;
		tmphstbuf = (char*) realloc (tmphstbuf, hstbuflen);
	}
	
	/*  Check for errors.  */
	bool succ = (res == 0 && hp != NULL);
	if( succ ) {
		memcpy((char*) &sa->sin_addr.s_addr, hp->h_addr, hp->h_length);
	}
	free( tmphstbuf );
	return succ;
}




///////////////////////////////////////////////////////////
// member funcs
EPosixClientSocket::EPosixClientSocket( EWrapper *ptr) : EClientSocketBase( ptr)
{
	m_fd = -1;
}

EPosixClientSocket::~EPosixClientSocket()
{
}

bool EPosixClientSocket::eConnect( const char *host, unsigned int port, int clientId)
{
	// reset errno
	errno = 0;

	// already connected?
	if( m_fd >= 0) {
		errno = EISCONN;
		getWrapper()->error( NO_VALID_ID, ALREADY_CONNECTED.code(), ALREADY_CONNECTED.msg());
		return false;
	}

	// initialize Winsock DLL (only for Windows)
	if ( !SocketsInit())	{
		return false;
	}

	// create socket
	m_fd = socket(AF_INET, SOCK_STREAM, 0);

	// cannot create socket
	if( m_fd < 0) {
		// uninitialize Winsock DLL (only for Windows)
		SocketsDestroy();
		getWrapper()->error( NO_VALID_ID, FAIL_CREATE_SOCK.code(), FAIL_CREATE_SOCK.msg());
		return false;
	}

	// use local machine if no host passed in
	if ( !( host && *host)) {
		host = "127.0.0.1";
	}

	// starting to connect to server
	struct sockaddr_in sa;
	memset( &sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons( port);
	sa.sin_addr.s_addr = inet_addr( host);

	if( !resolveHost( host, &sa ) ) {
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(),
			"Couldn't connect to TWS. Failed to resolve hostname.");
		return false;
	}

	// try to connect
	if( (connect( m_fd, (struct sockaddr *) &sa, sizeof( sa))) < 0) {
		// error connecting
		// uninitialize Winsock DLL (only for Windows)
		SocketsDestroy();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
		return false;
	}

	// set client id
	setClientId( clientId);

	onConnectBase();

	while( isSocketOK() && !isConnected()) {
		if ( !checkMessages()) {
			// uninitialize Winsock DLL (only for Windows)
			SocketsDestroy();
			getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
			return false;
		}
	}

	// successfully connected
	return true;
}

void EPosixClientSocket::eDisconnect()
{
	if ( m_fd >= 0 )
		// close socket
		SocketClose( m_fd);
	m_fd = -1;
	// uninitialize Winsock DLL (only for Windows)
	SocketsDestroy();
	eDisconnectBase();
}

bool EPosixClientSocket::isSocketOK() const
{
	return ( m_fd >= 0);
}

int EPosixClientSocket::fd() const
{
	return m_fd;
}

int EPosixClientSocket::send(const char* buf, size_t sz)
{
	if( sz <= 0)
		return 0;

	int nResult = ::send( m_fd, buf, sz, 0);

	if( nResult == -1 ) {
		return handleSocketError() ? 0 : -1;
	}
	return nResult;
}

int EPosixClientSocket::receive(char* buf, size_t sz)
{
	if( sz <= 0)
		return 0;

	int nResult = ::recv( m_fd, buf, sz, 0);

	if( nResult == -1 ) {
		return handleSocketError() ? 0 : -1;
	}
	if( nResult == 0) {
		//man 2 read: zero indicates EOF (e.g. socket disconnected)
		eDisconnect();
		getWrapper()->connectionClosed();
		return -1;
	}
	return nResult;
}

///////////////////////////////////////////////////////////
// callbacks from socket

void EPosixClientSocket::onReceive()
{
	checkMessages();
}

void EPosixClientSocket::onSend()
{
	sendBufferedData();
}

///////////////////////////////////////////////////////////
// helper
bool EPosixClientSocket::handleSocketError()
{
	// save and reset errno
	int errsv = errno;
	errno = 0;
	
	if( errsv == 0 || errsv == EWOULDBLOCK ) {
		return true;
	} else {
		getWrapper()->error( NO_VALID_ID, SOCKET_EXCEPTION.code(),
			SOCKET_EXCEPTION.msg() + strerror(errsv));
		
		eDisconnect();
		return false;
	}
}

} // namespace IB
