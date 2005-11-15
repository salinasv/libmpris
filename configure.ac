dnl Process this file with autoconf to produce a configure script.

AC_INIT([libmpris], [0.1])
AC_PREREQ([2.5])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

dnl Check for C compiler
AC_LANG([C])
AC_ISC_POSIX

dnl Checks for various programs
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Next four lines is a hack to prevent libtool checking for C++/F77
m4_undefine([AC_PROG_CXX])
m4_defun([AC_PROG_CXX],[])
m4_undefine([AC_PROG_F77])
m4_defun([AC_PROG_F77],[])

dnl X
AC_PATH_XTRA

dnl Libtool
AC_DISABLE_STATIC
AC_ENABLE_SHARED
AC_PROG_LIBTOOL

dnl GLIB
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.8.0 gobject-2.0 gthread-2.0 gmodule-2.0])

dnl GTK
PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.8.0])

dnl DBus
PKG_CHECK_MODULES([DBUS], [dbus-1 >= 0.35.2 dbus-glib-1 >= 0.35.2])

dnl XML stuff
    PKG_CHECK_MODULES([XML],
	[libxml-2.0 >= 2.6.17])

dnl GLADE 
    PKG_CHECK_MODULES([GLADE],
	[libglade-2.0])

dnl CFLAGS
CFLAGS="$CFLAGS -Wall -pedantic" # -std=c99 do not work with list_ stuff

dnl Example DEFINES 
MPRIS_DEFINES="-DDATA_DIR=\\\"\$(pkgdatadir)\\\""
AC_SUBST([MPRIS_DEFINES])

AC_CONFIG_FILES([
    Makefile
    libmpris/Makefile
    src/Makefile
    include/Makefile
    include/libmpris/Makefile
    include/libmpris/mpris/Makefile
])

AC_OUTPUT
