lib_LTLIBRARIES = libmpris.la

INCLUDES = \
	    $(GLIB_CFLAGS)		       \
	    $(XML_CFLAGS)		       \
	    $(DBUS_CFLAGS)		       \
	    -I$(top_builddir)/include/libmpris \
	    -DDBUS_API_SUBJECT_TO_CHANGE       \
	    -D__USE_GNU

libmpris_la_LDFLAGS = -export-dynamic 
libmpris_la_LIBADD = $(GLIB_CFLAGS) $(XML_LIBS) $(DBUS_LIBS)

libmpris_la_SOURCES = \
    connection.c  \
    dbus.c	  \
    mpris.c	  \
    procedures.c  
