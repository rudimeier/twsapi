/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"


#include "EPosixClientSocketPlatform.h"
#include "RudiClient.h"

#include "TwsSocketClientErrors.h"
#include "EWrapper.h"
#include "EDecoder.h"
#include "RudiReader.h"
#include "EMessage.h"

#include <string.h>
#include <assert.h>
#include <ostream>

#include "config.h"

const int MIN_SERVER_VER_SUPPORTED    = 38; //all supported server versions are defined in EDecoder.h

///// platform helpers ////////////////

static inline int
set_socket_nonblock(int sockfd)
{
#if defined _WIN32
	unsigned long mode = 1;
	if( ioctlsocket(sockfd, FIONBIO, &mode) != NO_ERROR ) {
		return -1;
	}
#else
	int flags = fcntl( sockfd, F_GETFL, 0 );
	if( flags == -1 ) {
		return -1;
	}
	if( fcntl(sockfd, F_SETFL, flags | O_NONBLOCK)  == -1 ) {
		return -1;
	}
#endif
	return 0;
}


static inline int
socket_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int rval = connect(sockfd, addr, addrlen );
#if defined _WIN32
	/* connect does not set errno on win32 */
	if( rval != 0 ) {
		errno = WSAGetLastError();
		if( errno == WSAEWOULDBLOCK ) {
			errno = EINPROGRESS;
		}
	}
#endif
	return rval;
}

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
RudiClient::RudiClient(EWrapper *ptr) : EClient( ptr, new ESocket())
{
	m_fd = SocketsInit() ? -1 : -2;
    m_allowRedirect = false;
    m_asyncEConnect = false;
}

RudiClient::~RudiClient()
{
	if( m_fd != -2)
		SocketsDestroy();
}

bool RudiClient::asyncEConnect() const {
    return m_asyncEConnect;
}

void RudiClient::asyncEConnect(bool val) {
    m_asyncEConnect = val;
}

bool RudiClient::eConnect( const char *host, unsigned int port, int clientId, bool extraAuth)
{
	/* after test period we'll change the default to AF_UNSPEC */
	return eConnect2( host, port, clientId, AF_INET, extraAuth );
}

/**
 * Same as eConnect() except you may the specify address family here (default is
 * AF_UNSPEC).
 * We couldn't just add the new family arg to eConnect because the original one
 * is pure virtual declared in EClientSocketBase. Thanks C++ design crap ...
 */
bool RudiClient::eConnect2( const char *host, unsigned int port,
	int clientId, int family, bool extraAuth )
{
	int con_errno = 0;
	int tmp;

	// already connected?
	if( m_fd >= 0) {
		assert(false); // for now we don't allow that
		goto end;
	}

	if( m_fd == -2) {
		getWrapper()->error( NO_VALID_ID, FAIL_CREATE_SOCK.code(), FAIL_CREATE_SOCK.msg());
		goto end;
	}

	// normalize host
	m_hostNorm = (host && *host) ? host : "127.0.0.1";

	// initialize host and port
	setHost( m_hostNorm);
	setPort( port);

	// starting to connect to server
	struct addrinfo *aitop;

	tmp = resolveHost( host, port, family, &aitop );
	if( tmp != 0 ) {
		const char *err;
#ifdef HAVE_GETADDRINFO
		err = gai_strerror(tmp);
#else
		err = "Invalid address, hostname resolving not supported.";
#endif
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		goto end;
	}

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
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		goto end;
	}

	getTransport()->fd(m_fd);

	// set client id
	setClientId( clientId);
	setExtraAuth( extraAuth);

	errno = 0;
	sendConnectRequest(); /* TODO return value check! */
	if( !getTransport()->isOutBufferEmpty() ) {
		/* For now we consider it as error if it's not possible to send an
		   integer string within a single tcp packet. Here we don't know weather
		   ::send() really failed or not. If so then we hopefully still have
		   it's errno set.*/
		const char *err = (errno != 0) ? strerror(errno)
			: "Sending client id failed.";
		eDisconnect();
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
		goto end;
	}

	if (!m_asyncEConnect) {
		/* TODO again we consider it as error if it's not possible to receive
		 * the connection ACK within one tcp packet. We need an onReceive()
		 * which processes exaclty only one message! */
		if( wait_socket( m_fd, WAIT_READ ) <= 0 ) {
			const char *err = (errno != 0) ? strerror(errno) : strerror(ENODATA);
			eDisconnect();
			getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), err );
			goto end;
		}

		/* TODO, stipid that we have to create our own Reder here. Moreover
		 * it's stupid in case !m_asyncEConnect to call user's connectAck()
		 * callback.*/
		RudiReader reader(this);
		reader.onReceive(); /* may disconnect us plus error callback */
		if (isConnected() && !m_serverVersion) {
			getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(),
				"couldn't get ack message from server" );
			eDisconnect(); /* although we may already disconnected */
			goto end;
		}
		assert( (!isConnected() && !m_serverVersion)
			|| (isConnected() && m_serverVersion) );
	}

end:
	fprintf(stderr, "CONNECT FINISHED ret:%d, isCon %d, connState: %d, async:%d\n",
			isSocketOK(), isConnected(), connState(), m_asyncEConnect);
	return isSocketOK();
}

ESocket *RudiClient::getTransport() {
    assert(dynamic_cast<ESocket*>(m_transport.get()) != 0);

    return static_cast<ESocket*>(m_transport.get());
}

void RudiClient::encodeMsgLen(std::string& msg, unsigned offset) const
{
	assert( !msg.empty());
	assert( m_useV100Plus);

	assert( sizeof(unsigned) == HEADER_LEN);
	assert( msg.size() > offset + HEADER_LEN);
	unsigned len = msg.size() - HEADER_LEN - offset;
	if( len > MAX_MSG_LEN) {
		m_pEWrapper->error( NO_VALID_ID, BAD_LENGTH.code(), BAD_LENGTH.msg());
		return;
	}

	unsigned netlen = htonl( len);
	memcpy( &msg[offset], &netlen, HEADER_LEN);
}

bool RudiClient::closeAndSend(std::string msg, unsigned offset)
{
	assert( !msg.empty());
	if( m_useV100Plus) {
		encodeMsgLen( msg, offset);
	}

	if (bufferedSend(msg) == -1)
        return handleSocketError();

    return true;
}

void RudiClient::prepareBufferImpl(std::ostream& buf) const
{
	assert( m_useV100Plus);
	assert( sizeof(unsigned) == HEADER_LEN);

	char header[HEADER_LEN] = { 0 };
	buf.write( header, sizeof(header));
}

void RudiClient::prepareBuffer(std::ostream& buf) const
{
	if( !m_useV100Plus)
		return;

	prepareBufferImpl( buf);
}

void RudiClient::eDisconnect()
{
	if ( m_fd >= 0 )
		// close socket
			SocketClose( m_fd);
	m_fd = -1;

	eDisconnectBase();
}

bool RudiClient::isSocketOK() const
{
	return ( m_fd >= 0);
}

int RudiClient::fd() const
{
	return m_fd;
}

int RudiClient::receive(char* buf, size_t sz)
{
	assert( sz > 0 );

	int nResult = ::recv( m_fd, buf, sz, 0);

	return nResult;
}

void RudiClient::serverVersion(int version, const char *time) {
    m_serverVersion = version;
    m_TwsTime = time;
	fprintf(stderr, "xserverVersion %d\n", version);
    if( usingV100Plus() ? (m_serverVersion < MIN_CLIENT_VER || m_serverVersion > MAX_CLIENT_VER) : m_serverVersion < MIN_SERVER_VER_SUPPORTED ) {
        eDisconnect();
        getWrapper()->error( NO_VALID_ID, UNSUPPORTED_VERSION.code(), UNSUPPORTED_VERSION.msg());
        return;
    }

	if (!m_asyncEConnect)
		startApi();
}

void RudiClient::redirect(const char *host, unsigned int port) {
	/* Original implementation was broken. Let's see if this will ever happen */
	getWrapper()->error( NO_VALID_ID, 9999,
		"WTF, got redirect request ... ignore and see what happens.");
}

bool RudiClient::handleSocketError()
{
	// no error
	if( errno == 0)
		return true;

	// Socket is already connected
	if( errno == EISCONN) {
		return true;
	}

	if( errno == EWOULDBLOCK)
		return false;

	if( errno == ECONNREFUSED) {
		getWrapper()->error( NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
	}
	else {
		getWrapper()->error( NO_VALID_ID, SOCKET_EXCEPTION.code(),
			SOCKET_EXCEPTION.msg() + strerror(errno));
	}
	// reset errno
	errno = 0;
	eDisconnect();
	return false;
}


///////////////////////////////////////////////////////////
// callbacks from socket

void RudiClient::onSend()
{
	/* TODO error handling? send() errno ... */
	getTransport()->sendBufferedData();
}

void RudiClient::onClose()
{
	eDisconnect();
	getWrapper()->connectionClosed();
}

void RudiClient::onError()
{
}
