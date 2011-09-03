AC_PREREQ([2.61])
AC_INIT([twsapi], [0.3.1], [http://www.ga-group.nl/])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_HEADER([./config.h])
AC_CONFIG_SRCDIR([PosixSocketClient/])

AM_INIT_AUTOMAKE([foreign])

## compiler check
AC_PROG_CXX
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_RANLIB

CPPFLAGS="${CPPFLAGS} -DIB_USE_STD_STRING"
CXXFLAGS=""
DEFAULT_CXXFLAGS="-g -Wall -W"
AC_SUBST([DEFAULT_CXXFLAGS])




ac_includes_default="$ac_includes_default
#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif
#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_WS2TCPIP_H
# include <ws2tcpip.h>
#endif
#ifdef HAVE_WSPIAPI_H
# include <wspiapi.h>
#endif"


dnl ** check for specific header (.h) files that we are interested in
AC_CHECK_HEADERS([fcntl.h winsock2.h ws2tcpip.h wspiapi.h])
AC_CHECK_HEADERS([arpa/inet.h netdb.h])

dnl --------------------------------------------------
dnl * add required libs for test client and pkg-config and the checks below
dnl --------------------------------------------------
if test "x$ac_cv_header_winsock2_h" = xyes; then
	LIBS="-lws2_32"
fi


dnl --------------------------------------------------
dnl * test for getaddrinfo
dnl --------------------------------------------------
AC_MSG_CHECKING(for getaddrinfo)
dnl Can't use AC_CHECK_FUNC here, because it doesn't do the right
dnl thing on Windows.
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[$ac_includes_default
int testme(){ getaddrinfo; }]])],[AC_DEFINE([HAVE_GETADDRINFO], [1], [Define to 1 if you have the `getaddrinfo' function.]) ac_have_getaddrinfo=yes; AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])

dnl Under mingw, we may need to set WINVER to 0x0501 to expose getaddrinfo.
if test "x$ac_have_getaddrinfo" = x; then
  old_CFLAGS="$CFLAGS"
  if test "z$ac_cv_header_ws2tcpip_h" = zyes; then
    CFLAGS="-DWINVER=0x0501 $CFLAGS"
    AC_MSG_CHECKING(for getaddrinfo if WINVER is 0x0501)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[$ac_includes_default
    int testme(){ getaddrinfo; }]])],[AC_DEFINE([HAVE_GETADDRINFO], [1], [Define to 1 if you have the `getaddrinfo' function.]) AC_DEFINE([NEED_WINVER_XP], [1], [Define to 1 if the `getaddrinfo' function needs WINVER set.]) EXTRA_CPPFLAGS="-DWINVER=0x0501 $EXTRA_CPPFLAGS"; AC_MSG_RESULT(yes)],[CFLAGS="$old_CFLAGS"; AC_MSG_RESULT(no)])
  fi
fi

dnl Missing under mingw, sigh.
AC_CHECK_FUNCS(gai_strerror)

dnl -------------------------------------------------------
dnl * test for AI_* flags that not all implementations have
dnl -------------------------------------------------------
AC_CHECK_DECLS([AI_ADDRCONFIG, AI_ALL, AI_NUMERICSERV, AI_V4MAPPED])





## output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([PosixSocketClient/Makefile])
AC_CONFIG_FILES([TestPosixSocketClient/Makefile])
AC_CONFIG_FILES([libtwsapi.pc])
AC_OUTPUT

