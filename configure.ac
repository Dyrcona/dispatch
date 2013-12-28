#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([dispatch], [2.0.0], [jason@sigio.com], [],
        [http://www.sigio.com/dispatch/])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_AUX_DIR([m4])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 lib/Makefile
                 src/Makefile
		 man/Makefile])
AC_OUTPUT
