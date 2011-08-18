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

## output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([PosixSocketClient/Makefile])
AC_CONFIG_FILES([TestPosixSocketClient/Makefile])
AC_CONFIG_FILES([libtwsapi.pc])
AC_OUTPUT

