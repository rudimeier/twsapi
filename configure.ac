AC_PREREQ([2.61])
AC_INIT([twsapi], [0.6.0], [http://www.ga-group.nl/])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_HEADER([./config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([PosixSocketClient/])

AM_INIT_AUTOMAKE([foreign])
LT_INIT([disable-shared])

## compiler check
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_CXX
AC_PROG_CXX_C_O

AC_LANG([C++])
AX_COMPILER_VENDOR

CPPFLAGS="${CPPFLAGS} -DIB_USE_STD_STRING"
# enable some warnings
AX_CHECK_COMPILE_FLAG([-Wall],[warnflags="-Wall"])
# intel's -Wextra is too verbose
if test "$ax_cv_cxx_compiler_vendor" != "intel"; then
	AX_CHECK_COMPILE_FLAG([-Wextra],[warnflags="$warnflags -Wextra"])
fi
DEFAULT_CXXFLAGS="$warnflags"
AC_SUBST([DEFAULT_CXXFLAGS])


dnl ** our default includes should support networking stuff
ac_includes_default="$ac_includes_default
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif
#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_WS2TCPIP_H
# include <ws2tcpip.h>
#endif"


dnl ** check for specific header (.h) files that we are interested in
AC_CHECK_HEADERS([fcntl.h errno.h])
AC_CHECK_HEADERS([arpa/inet.h sys/socket.h netdb.h])
AC_CHECK_HEADERS([winsock2.h ws2tcpip.h],
  [LIBS="-lws2_32"])


dnl ** some systems need better checks for get*info
SANE_CHECK_NETDB_FUNC(getaddrinfo)
SANE_CHECK_NETDB_FUNC(gai_strerror)


dnl ** test for AI_* flags that not all implementations have
AC_CHECK_DECLS([AI_ADDRCONFIG, AI_ALL, AI_NUMERICSERV, AI_V4MAPPED])
AC_CHECK_DECLS([ENODATA, ETIMEDOUT])

## offload some of the final targets here
headerdir="${includedir}/${PACKAGE}"
AC_SUBST([headerdir])

## output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([PosixSocketClient/Makefile])
AC_CONFIG_FILES([TestPosixSocketClient/Makefile])
AC_CONFIG_FILES([twsapi.pc])
AC_CONFIG_FILES([twsapi.spec])
AC_OUTPUT
