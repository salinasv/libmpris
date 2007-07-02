bin_PROGRAMS = mpris_demo 

transform = s/_/-/g

mpris_demo_LDFLAGS = -Wl, --export-dynamic

mpris_demo_LDADD = \
	$(DBUS_LIBS)			      \
	$(top_builddir)/libmpris/libmpris.la

mpris_demo_CFLAGS = \
	-I$(top_srcdir)                              \
	-I$(top_srcdir)/include/libmpris             \
	-DG_LOG_DOMAIN=\"MPRIS\"                     \
	$(DBUS_CFLAGS)				     \
	$(MPRIS_DEFINES)

mpris_demo_SOURCES = \
	 mpris-demo.c
