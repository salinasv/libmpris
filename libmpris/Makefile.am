lib_LTLIBRARIES = libmpris.la

INCLUDES = \
		$(DBUS_CFLAGS) 				\
		$(CFLAGS) 					\
		-I$(top_builddir)/include 	\
		-DDBUS_API_SUBJECT_TO_CHANGE 		\
		-D__USE_GNU

libmpris_la_LDFLAGS = -export-dynamic 
libmpris_la_LIBADD = $(DBUS_LIBS) -lpthread

libmpris_la_SOURCES = 	\
	dbus.c				\
	mpris.c	  			\
	list.c						

