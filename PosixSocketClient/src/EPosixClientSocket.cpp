#include "EPosixClientSocket.h"

#include "EPosixClientSocketPlatform.h"
#include "TwsSocketClientErrors.h"
#include "EWrapper.h"

#include <string.h>
#include <assert.h>
#include <fcntl.h>

namespace IB {

///////////////////////////////////////////////////////////
// static helper
bool resolveHost( const char *host, sockaddr_in *sa )
{
	if (sa->sin_addr.s_addr != INADDR_NONE) {
		/* No need to resolve it. */
		return true;
	}

#ifdef __CYGWIN__
	return false;
#else
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
#endif
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


enum { WAIT_READ = 1, WAIT_WRITE = 2 };

int EPosixClientSocket::wait_socket( int flag )
{
	errno = 0;
	const int timeout_msecs = 5000;
	
	struct timeval tval;
	tval.tv_usec = 1000 * (timeout_msecs % 1000);
	tval.tv_sec = timeout_msecs / 1000;

	fd_set waitSet;
	FD_ZERO( &waitSet );
	FD_SET( m_fd, &waitSet );

	int ret;
	switch( flag ) {
	case WAIT_READ:
		ret = select( m_fd + 1, &waitSet, NULL, NULL, &tval );
		break;
	case WAIT_WRITE:
		ret = select( m_fd + 1, NULL, &waitSet, NULL, &tval );
		break;
	default:
		assert( false );
		ret = 0;
		break;
	}
	
	return ret;
}


bool EPosixClientSocket::eConnect( const char *host, unsigned int port, int clientId)
{
	// already connected?
	if( m_fd >= 0) {
		assert(false); // for now we don't allow that
		return true;
	}

	// initialize Winsock DLL (only for Windows)
	if ( !SocketsInit())	{
		// Does this set errno?
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(),
			"Initializing Winsock DLL failed.");
		return false;
	}

	// create socket
	m_fd = socket(AF_INET, SOCK_STREAM, 0);

	// cannot create socket
	if( m_fd < 0) {
		const char *err = strerror(errno);
		// uninitialize Winsock DLL (only for Windows)
		SocketsDestroy();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	/* Set socket O_NONBLOCK. If wanted we could handle errors (portability!).
	   We could even make O_NONBLOCK optional. */
	int flags = fcntl( m_fd, F_GETFL, 0 );
	assert( flags >= 0 );
	if( fcntl(m_fd, F_SETFL, flags | O_NONBLOCK)  < 0 ) {
		assert( false );
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
		if( errno != EINPROGRESS ) {
			const char *err = strerror(errno);
			eDisconnect();
			getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
			return false;
		}
	}
	if( wait_socket( WAIT_WRITE  ) <= 0 ) {
		const char *err = (errno != 0) ? strerror(errno) : strerror(ETIMEDOUT);
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	// set client id
	setClientId( clientId);

	errno = 0;
	onConnectBase();
	if( !isOutBufferEmpty() ) {
		/* For now we consider it as error if it's not possible to send an
		   integer string within a single tcp packet. Here we don't know weather
		   ::send() really failed or not. If so then we hopefully still have
		   it's errno set. Seems that we even get ECONNREFUSED (O_NONBLOCK). */
		const char *err = (errno != 0) ? strerror(errno)
			: "Sending client id failed.";
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	if( wait_socket( WAIT_READ ) <= 0 ) {
		const char *err = (errno != 0) ? strerror(errno) : strerror(ENODATA);
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	while( !isConnected() ) {
		assert( isSocketOK() ); // need to be handled if send() would destroy it
		if ( !checkMessagesConnect()) {
			const char *err = (errno != 0) ? strerror(errno)
				: "The remote host closed the connection.";
			eDisconnect();
			getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
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
	assert( sz > 0 );

	int nResult = ::send( m_fd, buf, sz, 0);

	if( nResult == -1 ) {
		if( isConnected() ) {
			const char *err = strerror(errno);
			getWrapper()->error( NO_VALID_ID, SOCKET_EXCEPTION.code(), err );
			eDisconnect();
			getWrapper()->connectionClosed();
		} else {
			/* will be handled within eConnect() ... */
		}
	}

	return nResult;
}

int EPosixClientSocket::receive(char* buf, size_t sz)
{
	assert( sz > 0 );

	int nResult = ::recv( m_fd, buf, sz, 0);

	return nResult;
}

///////////////////////////////////////////////////////////
// callbacks from socket

void EPosixClientSocket::onReceive()
{
	if( !checkMessages() ) {
		const char * err = (errno != 0) ? strerror(errno)
			: "The remote host closed the connection.";
		getWrapper()->error( NO_VALID_ID, SOCKET_EXCEPTION.code(), err );
		eDisconnect();
		getWrapper()->connectionClosed();
	}
}

void EPosixClientSocket::onSend()
{
	sendBufferedData();
}


} // namespace IB
