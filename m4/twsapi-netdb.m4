dnl netdb.m4 --- getaddrinfo WINVER magic
dnl
dnl Copyright (C) 2012 Ruediger Meier
dnl
dnl Author: Ruediger Meier <sweet_f_a@s.org>
dnl License: BSD 3-Clause
dnl
dnl This file is part of twsapi.

#
# Checks for functions which usually come from netdb.h or ws2tcpip.h (e.g.
# getaddrinfo). Header checks have to be done before as well as adding the right
# libs.
AC_DEFUN([SANE_CHECK_NETDB_FUNC],
[
  # FIXME make this work with C++
  AC_LANG_PUSH([C])
  do_winver_test="no"

  AC_CACHE_CHECK([for $1],[ac_cv_func_$1],
    [AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM(
        [$ac_includes_default],
        [int testme(){ $1; }])],
      [ac_cv_func_$1="yes"],
      [ac_cv_func_$1="no"
      do_winver_test="yes"])])

  dnl Under mingw, we may need to set WINVER to 0x0501 to expose the function.
  if test "$do_winver_test" = "yes"; then
    AC_MSG_CHECKING([for win32 ]$1[ if WINVER is 0x0501])
    AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM(
        [#define WINVER 0x0501
        $ac_includes_default],
        [int testme(){ $1; }])],
      [AC_MSG_RESULT(yes)
      ac_cv_func_$1="yes"
      AC_DEFINE([NEED_WINVER_XP], [1],
        [Define to 1 if some win32 functions needs WINVER set.])],
      [AC_MSG_RESULT(no)
      ac_cv_func_$1="no"])
  fi

  if test "$ac_cv_func_$1" = "yes"; then
    AC_DEFINE([HAVE_]translit($1, [a-z], [A-Z]), [1],
      [Define to 1 if you have the `]$1[' function.])
  fi

  unset do_winver_test
  AC_LANG_POP([C])
])
