AC_PREREQ([2.61])
AC_INIT([twsapi], [1.973.0], [http://www.ga-group.nl/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADER([./config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([cpp/client/Contract.h])

AM_INIT_AUTOMAKE([-Wall foreign dist-xz no-dist-gzip subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT([disable-shared])

## compiler check
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_CXX
AC_PROG_CXX_C_O

AC_LANG([C++])
AX_COMPILER_VENDOR
AX_CXX_COMPILE_STDCXX([11],[noext])

# enable some warnings
AX_CHECK_COMPILE_FLAG([-Wall],[warnflags="-Wall"])
AX_CHECK_COMPILE_FLAG([-Wextra],[warnflags="$warnflags -Wextra"])
# intel's -Wextra is too verbose, disable some remarks
if test "$ax_cv_cxx_compiler_vendor" = "intel"; then
    AX_CHECK_COMPILE_FLAG([-diag-disable=11074],[warnflags="$warnflags -diag-disable=11074,11076"])
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

AC_ARG_ENABLE([iborig],
  AS_HELP_STRING([--enable-iborig], [build the original EClientSocket]),
  [], [enable_iborig=check])
have_pthreads=no
AS_IF([test "x$enable_iborig" != xno], [
  have_pthreads=yes
  AS_CASE([$enable_iborig:$have_pthreads],
    [yes:no], [AC_MSG_ERROR([iborig requested but no pthread support])],
    [*:yes], AC_DEFINE([WITH_IBORIG], [1], [Define if we build with iborig enabled]))])
AM_CONDITIONAL([WITH_IBORIG], [test "x$have_pthreads" = xyes])

AC_ARG_ENABLE([ssl],
  AS_HELP_STRING([--enable-ssl], [build with ssl support]),
  [], [enable_ssl=check])
have_ssl=no
AS_IF([test "x$enable_ssl" != xno], [
  PKG_CHECK_MODULES([openssl], [openssl], [have_ssl=yes], [have_ssl=no])
  AS_CASE([$enable_ssl:$have_ssl],
    [yes:no], [AC_MSG_ERROR([ssl requested but not found])],
    [*:yes], AC_DEFINE([HAVE_SSL], [1], [Define if openssl is available]))])
AM_CONDITIONAL([HAVE_SSL], [test "x$have_ssl" = xyes])

## offload some of the final targets here
headerdir="${includedir}/${PACKAGE}"
AC_SUBST([headerdir])

## output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([cpp/Makefile])
AC_CONFIG_FILES([twsapi.pc])
AC_CONFIG_FILES([twsapi-iborig.pc])
AC_CONFIG_FILES([twsapi.spec])
AC_OUTPUT
