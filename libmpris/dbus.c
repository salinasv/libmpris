#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glade/glade.h>

#include <libmpris/mpris.h>

static  DBusGConnection *bus	   = NULL;
static  DBusGProxy	*busproxy  = NULL;
static  GError		*error	   = NULL;

gboolean
mpris_dbus_init (void)
{
  bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);

  if (!bus)
    {
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "Couldn't connect to session bus: %s", error->message);
      return FALSE;
    }

  bus_proxy = dbus_g_proxy_new_for_name (bus, "org.freedesktop.DBus",
					      "/org/freedesktop/DBus",
					      "org.freedesktop.DBus");
}

GPtrArray*
mpris_dbus_list (void)
{
  GPtrArray  *players = NULL;
  gchar	    **names = NULL
  gint	      n = 0;

  if (!dbus_g_proxy_call (bus_proxy, "ListNames", &error,
                          G_TYPE_INVALID,
                          G_TYPE_STRV, 
			  &names,
                          G_TYPE_INVALID))
    {
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "Couldn't run ListNames on bus: %s", error->message);
      return NULL;
    }
 
  while (names[n])
    {
      g_message ("%s: name: '%s'", names[n]);
      n++;
    }
}
