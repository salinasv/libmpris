#include <glib.h>
#include <glib/gi18n.h>

#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>

static  DBusGConnection *bus	   = NULL;
static  DBusGProxy	*bus_proxy = NULL;
static  GError		*error	   = NULL;

#define MPRIS_INTERFACE_PREFIX	"org.mpris"

gboolean
mpris_dbus_init (void)
{
  dbus_g_type_specialized_init ();

  bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);

  if (!bus)
    {
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "Couldn't connect to session bus: %s", error->message);
      return FALSE;
    }

  bus_proxy = dbus_g_proxy_new_for_name (bus, "org.freedesktop.DBus",
					      "/org/freedesktop/DBus",
					      "org.freedesktop.DBus");
  
  if (!bus_proxy)
    // FIXME: Print error
    return FALSE;

  return TRUE;
}

GPtrArray*
mpris_dbus_list (void)
{
  GPtrArray  *players = NULL;
  gchar	    **names = NULL;
  gint	      n = 0;
  gint	     id = 0;

  if (!dbus_g_proxy_call (bus_proxy, "ListNames", &error,
                          G_TYPE_INVALID,
                          G_TYPE_STRV, 
			  &names,
                          G_TYPE_INVALID))
    {
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "Couldn't run ListNames on bus: %s", error->message);
      return NULL;
    }
  
  players = g_ptr_array_new ();
 
  while (names[n])
    {
      if (!g_ascii_strncasecmp (MPRIS_INTERFACE_PREFIX, names[n], strlen(MPRIS_INTERFACE_PREFIX)))
	{
	  MPRISPlayerInfo *p_info = g_new0 (MPRISPlayerInfo,1);

	  p_info->id = id;
	  p_info->name = g_strdup (names[n]);
	  p_info->interface = g_strdup (names[n]);

	  g_ptr_array_add (players, p_info);
	  id++;
	}
      n++;
    }

  g_strfreev (names);

  return players;
}
