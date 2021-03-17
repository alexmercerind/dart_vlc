m4_define([VLCPP_MAJOR], [0])
m4_define([VLCPP_MINOR], [1])
m4_define([VLCPP_MICRO], [0])

AC_COPYRIGHT([Copyright 2014-2016 VideoLAN - VideoLabs])

AC_INIT([libvlcpp], [VLCPP_MAJOR.VLCPP_MINOR.VLCPP_MICRO])

AM_INIT_AUTOMAKE([foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [
  AM_SILENT_RULES([yes])
])

AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext])

AC_ARG_ENABLE(examples, AC_HELP_STRING([--enable-examples], [build examples programs]))
AM_CONDITIONAL([HAVE_EXAMPLES], [test "${enable_examples}" = "yes"])
AS_IF([test "${enable_examples}" = "yes"], [PKG_CHECK_MODULES(vlc, libvlc)])

AC_ARG_ENABLE(werror, AC_HELP_STRING([--enable-werror], [build examples with -Werror]))
AM_CONDITIONAL([HAVE_WERROR], [test "${enable_werror}" = "yes"])

AC_CONFIG_FILES([
Makefile
libvlcpp.pc
])

AC_OUTPUT
