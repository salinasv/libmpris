bin_PROGRAMS = mpris_demo 

transform = s/_/-/g

mpris_demo_LDFLAGS = -Wl, --export-dynamic

mpris_demo_LDADD = \
	$(GTK_LIBS) \
	$(GLADE_LIBS) \
	$(DBUS_LIBS)			      \
	$(top_builddir)/libmpris/libmpris.la

mpris_demo_CFLAGS = \
	-I$(top_srcdir)                              \
	-I$(top_srcdir)/include/libmpris             \
	-DG_LOG_DOMAIN=\"MPRIS\"                     \
	-DDBUS_API_SUBJECT_TO_CHANGE		     \
	$(GTK_CFLAGS)				     \
	$(GLADE_CFLAGS)				     \
	$(DBUS_CFLAGS)				     \
	$(MPRIS_DEFINES)

mpris_demo_SOURCES = \
	 mpris-demo.c

gladedir = $(pkgdatadir)/glade

glade_DATA = \
	 mpris-demo.glade
EXTRA_DIST =
	 mpris-demo.glade
