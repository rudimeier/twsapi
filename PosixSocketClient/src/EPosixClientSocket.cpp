/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "EPosixClientSocket.h"

#include "config.h"
#include "EPosixClientSocketPlatform.h"
#include "TwsSocketClientErrors.h"
#include "EWrapper.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

namespace IB {

/**
 * Resolve host names.
 * Return 0 on success or EAI_* errcode to be used with gai_strerror().
 */
static int resolveHost( const char *host, unsigned int port, int family,
	struct addrinfo **res )
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = family;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
#if HAVE_DECL_AI_ADDRCONFIG
	hints.ai_flags |= AI_ADDRCONFIG;
#endif
#if HAVE_DECL_AI_V4MAPPED
	hints.ai_flags |= AI_V4MAPPED;
#endif
	hints.ai_protocol = 0;

	char strport[32];
	/* Convert the port number into a string. */
	snprintf(strport, sizeof strport, "%u", port);

	int s = getaddrinfo(host, strport, &hints, res);
	return s;
}


enum { WAIT_READ = 1, WAIT_WRITE = 2 };

static int wait_socket( int fd, int flag )
{
	errno = 0;
	const int timeout_msecs = 5000;

	struct timeval tval;
	tval.tv_usec = 1000 * (timeout_msecs % 1000);
	tval.tv_sec = timeout_msecs / 1000;

	fd_set waitSet;
	FD_ZERO( &waitSet );
	FD_SET( fd, &waitSet );

	int ret;
	switch( flag ) {
	case WAIT_READ:
		ret = select( fd + 1, &waitSet, NULL, NULL, &tval );
		break;
	case WAIT_WRITE:
#if defined _WIN32
		/* exceptfds is only needed on windows to note "connection refused "*/
		fd_set exceptfds;
		FD_ZERO( &exceptfds );
		FD_SET( fd, &exceptfds );
		ret = select( fd + 1, NULL, &waitSet, &exceptfds, &tval );
#else
		ret = select( fd + 1, NULL, &waitSet, NULL, &tval );
#endif
		break;
	default:
		assert( false );
		ret = 0;
		break;
	}

	return ret;
}

static int timeout_connect( int fd, const struct sockaddr *serv_addr,
	socklen_t addrlen )
{
	if( socket_connect( fd, serv_addr, addrlen) < 0 ) {
		if( errno != EINPROGRESS ) {
			return -1;
		}
	}
	if( wait_socket( fd, WAIT_WRITE  ) <= 0 ) {
		if( errno == 0 ) {
			errno = ETIMEDOUT;
		}
		return -1;
	}

	/* Completed or failed */
	int optval = 0;
	socklen_t optlen = sizeof(optval);
	/* casting  &optval to char* is required for win32 */
	if( getsockopt(fd, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) == -1 ) {
		return -1;
	}
	if (optval != 0) {
		errno = optval;
		return -1;
	}
	return 0;
}



///////////////////////////////////////////////////////////
// member funcs
EPosixClientSocket::EPosixClientSocket( EWrapper *ptr) : EClientSocketBase( ptr)
{
	m_fd = -1;
	hnd_shk_state = HND_SHK_ST_UNK;
}

EPosixClientSocket::~EPosixClientSocket()
{
}

bool EPosixClientSocket::eConnect( const char *host, unsigned int port, int clientId)
{
	/* after test period we'll change the default to AF_UNSPEC */
	return eConnect2( host, port, clientId, AF_INET );
}

/**
 * Same as eConnect() except you may the specify address family here (default is
 * AF_UNSPEC).
 * We couldn't just add the new family arg to eConnect because the original one
 * is pure virtual declared in EClientSocketBase. Thanks C++ design crap ...
 */
bool EPosixClientSocket::eConnect2( const char *host, unsigned int port,
	int clientId, int family )
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

	// use local machine if no host passed in
	if ( !( host && *host)) {
		host = "127.0.0.1";
	}

	// starting to connect to server
	struct addrinfo *aitop;

	int s = resolveHost( host, port, family, &aitop );
	if( s != 0 ) {
		SocketsDestroy();
		const char *err;
#ifdef HAVE_GETADDRINFO
		err = gai_strerror(s);
#else
		err = "Invalid address, hostname resolving not supported.";
#endif
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	int con_errno = 0;
	for( struct addrinfo *ai = aitop; ai != NULL; ai = ai->ai_next ) {

		// create socket
		m_fd = socket(ai->ai_family, ai->ai_socktype, 0);
		if( m_fd < 0) {
			con_errno = errno;
			continue;
		}

		/* Set socket O_NONBLOCK. If wanted we could handle errors
		   (portability!) We could even make O_NONBLOCK optional. */
		int sn = set_socket_nonblock( m_fd );
		assert( sn == 0 );

		// try to connect
		if( timeout_connect( m_fd, ai->ai_addr, ai->ai_addrlen ) < 0 ) {
			con_errno = errno;
			SocketClose(m_fd);
			m_fd = -1;
			continue;
		}
		/* successfully  connected */
		break;
	}

	freeaddrinfo(aitop);

	/* connection failed, tell the error which happened in our last try  */
	if( m_fd < 0 ) {
		const char *err = strerror(con_errno);
		SocketsDestroy();
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
		   it's errno set.*/
		const char *err = (errno != 0) ? strerror(errno)
			: "Sending client id failed.";
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		return false;
	}

	if( wait_socket( m_fd, WAIT_READ ) <= 0 ) {
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
	switch (this->hnd_shk_state) {
	default:
	case HND_SHK_ST_UNK:
		if ( m_fd >= 0 ) {
			// close socket
			SocketClose( m_fd);
		}
		// uninitialize Winsock DLL (only for Windows)
		SocketsDestroy();
		/*@fallthrough@*/
	case HND_SHK_ST_CLEAN:
	case HND_SHK_ST_SENT_TOKEN:
	case HND_SHK_ST_RCVD_CONNACK:
		this->hnd_shk_state = HND_SHK_ST_UNK;
		m_fd = -1;
		eDisconnectBase();
		break;
	}
}

bool EPosixClientSocket::isSocketOK() const
{
	return ( m_fd >= 0);
}

int EPosixClientSocket::fd() const
{
	return m_fd;
}


int EPosixClientSocket::prepareHandshake(int socket, int clientId)
{
	if (this->m_fd > 0) {
		/* don't matter what SOCKET is we use the one we know about */
		;
	} else if ((this->m_fd = socket) < 0) {
		errno = EBADF;
		return -1;
	} else {
		/* set the client id also */
		setClientId(clientId);
		this->hnd_shk_state = HND_SHK_ST_CLEAN;
	}
	return 0;
}

int EPosixClientSocket::handshake(void)
{
/* if everything goes ok, handshake() will return 0,
 * on error -1 is returned, and
 * once the handshake is finished 1 is returned. */
	if (this->m_fd < 0) {
		/* do fuckall */
		errno = EBADF;
		return -1;
	}

	switch (this->hnd_shk_state) {
	case HND_SHK_ST_CLEAN:
		// initiate the handshake
		onConnectBase();
		if (!isOutBufferEmpty()) {
			// let the user deal with this
			// errno is hopefully still set
			return -1;
		}
		this->hnd_shk_state = HND_SHK_ST_SENT_TOKEN;
		break;

	case HND_SHK_ST_SENT_TOKEN:
		if (!checkMessagesConnect()) {
			// great effort, why don't we reboot the computer now?
			eDisconnectBase();
			this->hnd_shk_state = HND_SHK_ST_UNK;
			return -1;
		}
		this->hnd_shk_state = HND_SHK_ST_RCVD_CONNACK;
		/*@fallthrough@*/

	case HND_SHK_ST_RCVD_CONNACK:
		/* handshake succeeded */
		return 1;
	default:
	case HND_SHK_ST_UNK:
		return -1;
	}

	// successfully connected
	return 0;
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
	/* as a special service, complete the handshake here */
	switch (this->hnd_shk_state) {
	case HND_SHK_ST_SENT_TOKEN:
		handshake();
	case HND_SHK_ST_CLEAN:
		return;
	case HND_SHK_ST_RCVD_CONNACK:
	default:
	case HND_SHK_ST_UNK:
		break;
	}

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
	/* as a special service, complete the handshake here */
	switch (this->hnd_shk_state) {
	case HND_SHK_ST_CLEAN:
		handshake();
	case HND_SHK_ST_SENT_TOKEN:
		return;
	case HND_SHK_ST_RCVD_CONNACK:
	default:
	case HND_SHK_ST_UNK:
		break;
	}

	sendBufferedData();
}


} // namespace IB
