bin_PROGRAMS = mpris_demo mpris_list mpris_control

transform = s/_/-/g

COMMON_LDFLAGS = -Wl, --export-dynamic

COMMON_LDADD = 		\
	$(DBUS_LIBS) 	\
	$(top_builddir)/libmpris/libmpris.la

COMMON_CFLAGS = 				\
	-I$(top_srcdir) 			\
	-I$(top_srcdir)/include/  	\
	-DG_LOG_DOMAIN=\"MPRIS\"   	\
	$(DBUS_CFLAGS)				\
	$(MPRIS_DEFINES)			\
	-D__USE_BSD

mpris_demo_LDFLAGS = $(COMMON_LDFLAGS) 
mpris_demo_LDADD = $(COMMON_LDADD)	
mpris_demo_CFLAGS = $(COMMON_CFLAGS)
mpris_demo_SOURCES = \
	 mpris-demo.c

mpris_list_LDFLAGS = $(COMMON_LDFLAGS) 
mpris_list_LDADD = $(COMMON_LDADD)	
mpris_list_CFLAGS = $(COMMON_CFLAGS)
mpris_list_SOURCES = \
	 mpris-list.c

mpris_control_LDFLAGS = $(COMMON_LDFLAGS) 
mpris_control_LDADD = $(COMMON_LDADD)	
mpris_control_CFLAGS = $(COMMON_CFLAGS)
mpris_control_SOURCES = \
	 mpris-control.c
