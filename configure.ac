#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([dispatch], [2.0.0], [jason@sigio.com], [],
        [http://www.sigio.com/dispatch/])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_AUX_DIR([m4])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Check for boost headers and libraries that we use.
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS([boost/locale.hpp boost/program_options.hpp \
                  boost/iostreams/concepts.hpp],[],
                  [AC_MSG_ERROR([Cannot locate a required boost header.
You may need to set --includedir when running configure, or you may need
to set the various --with-libboost_*-prefix options.
See ./configure --help for more information.])])
AC_LIB_LINKFLAGS([boost_locale])
AC_LIB_LINKFLAGS([boost_program_options])
AC_LIB_LINKFLAGS([boost_iostreams])
AC_LANG_POP(C++)

# Finalize the po/Makefile:
AM_PO_SUBDIRS

AC_CONFIG_FILES([Makefile po/Makefile.in
                 include/Makefile
                 lib/Makefile
                 src/Makefile
		 man/Makefile])
AC_OUTPUT
