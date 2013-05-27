/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef eposixclientsocketcommon_def
#define eposixclientsocketcommon_def

#include "config.h"

#ifdef _WIN32
	// Windows
	// includes
	#ifdef NEED_WINVER_XP
		#define WINVER 0x0501
	#endif
	# ifdef HAVE_WS2TCPIP_H
		#include <ws2tcpip.h>
	# endif

	#include <winsock2.h>
	#include <time.h>

	/* These errno re-defines are bad because we want to use strerror on them.
	   We would need to rewrite strerror for _WIN32 but currently I just want
	   to finish with that crap quick and dirty. */
	#define EISCONN WSAEISCONN
	#define EWOULDBLOCK WSAEWOULDBLOCK
	#define ECONNREFUSED WSAECONNREFUSED

	#define EINPROGRESS WSAEINPROGRESS
	#define ETIMEDOUT WSAETIMEDOUT
	#define ENODATA WSANO_DATA

	namespace IB {
	// helpers
	inline bool SocketsInit( void) {
		WSADATA data;
		return ( !WSAStartup( MAKEWORD(2, 2), &data));
	};
	inline bool SocketsDestroy() { return ( !WSACleanup()); };
	inline int SocketClose(int sockfd) { return closesocket( sockfd); };
	}

#else
	// LINUX
	// includes
	#include <arpa/inet.h>
	#include <errno.h>
	#include <sys/select.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#ifdef HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
	#ifdef HAVE_SYS_SOCKET_H
		#include <sys/socket.h>
	#endif

	#if ! HAVE_DECL_ENODATA
		#define ENODATA ETIMEDOUT
	#endif

	namespace IB {
	// helpers
	inline bool SocketsInit() { return true; };
	inline bool SocketsDestroy() { return true; };
	inline int SocketClose(int sockfd) { return close( sockfd); };
	}

#endif


namespace IB {


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


} // namespace IB


#endif
