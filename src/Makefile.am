bin_PROGRAMS = mpris_demo mpris_list mpris_control

transform = s/_/-/g

AM_LDFLAGS = -Wl, --export-dynamic

AM_LDADD = 		\
	$(DBUS_LIBS) 	\
	$(top_builddir)/libmpris/libmpris.la

AM_CFLAGS = 				\
	-I$(top_srcdir) 			\
	-I$(top_srcdir)/include/  	\
	-DG_LOG_DOMAIN=\"MPRIS\"   	\
	$(DBUS_CFLAGS)				\
	$(MPRIS_DEFINES)			\
	-D__USE_BSD

mpris_demo_LDFLAGS = $(AM_LDFLAGS) 
mpris_demo_LDADD = $(AM_LDADD)	
mpris_demo_CFLAGS = $(AM_CFLAGS)
mpris_demo_SOURCES = \
	 mpris-demo.c

mpris_list_LDFLAGS = $(AM_LDFLAGS) 
mpris_list_LDADD = $(AM_LDADD)	
mpris_list_CFLAGS = $(AM_CFLAGS)
mpris_list_SOURCES = \
	 mpris-list.c

mpris_control_LDFLAGS = $(AM_LDFLAGS) 
mpris_control_LDADD = $(AM_LDADD)	
mpris_control_CFLAGS = $(AM_CFLAGS)
mpris_control_SOURCES = \
	 mpris-control.c
