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

	static int set_socket_nonblock(int sockfd)
		{
			unsigned long non_zero = 123;
			if( ioctlsocket( sockfd, FIONREAD,  &non_zero) == NO_ERROR ) {
				return 0;
			}
			return -1;
		}
	}

#else
	// LINUX
	// includes
	#include <arpa/inet.h>
	#include <errno.h>
	#include <sys/select.h>
	#include <netdb.h>
	#include <fcntl.h>

	namespace IB {
	// helpers
	inline bool SocketsInit() { return true; };
	inline bool SocketsDestroy() { return true; };
	inline int SocketClose(int sockfd) { return close( sockfd); };

	static int set_socket_nonblock(int sockfd)
	{
		int flags = fcntl( sockfd, F_GETFL, 0 );
		if( flags < 0 ) {
			return -1;
		}
		if( fcntl(sockfd, F_SETFL, flags | O_NONBLOCK)  < 0 ) {
			return -1;
		}
		return 0;
	}
	}

#endif

#endif
