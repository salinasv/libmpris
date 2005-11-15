#include <glib.h>
#include <glib-object.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

#if 0
struct _MPRISConnection {
      MPRISPlayerInfo *info;
      DBusGProxy      *player;
};
#endif

static gchar* method_names[] = {
      "PlayNext",
      "PlayPrev",
      "PlayPause",
      "PlayStop",
      "PlayCurrent"
};

#if 0
MPRISConnection*
mpris_connection_establish (MPRISPlayerInfo *info)
{
    MPRISConnection *conn = g_new0 (MPRISConnection,1);

    conn->player = dbus_g_proxy_new_for_name (mpris_bus,
					      info->interface, 
					      info->path,
					      info->interface); 

    if (!conn->player)
      {
	g_free (conn);
	return NULL;
      }

    conn->info = info; 

    return conn;
}

gboolean
mpris_connection_invoke_method (MPRISConnection *connection, MPRISMethodId method_id)
{
     switch (method_id) 
      {
	case MPRIS_METHOD_PLAY_NEXT:
	case MPRIS_METHOD_PLAY_PREV:
	case MPRIS_METHOD_PLAY_PAUSE:
	case MPRIS_METHOD_PLAY_STOP:
	case MPRIS_METHOD_PLAY_CURRENT:
	  {
	      GError	    *error = NULL;

	      if (!dbus_g_proxy_call (connection->player,
				      method_names[method_id], 
				      &error,
				      G_TYPE_INVALID, 
				      G_TYPE_INVALID))
		{
		  g_message(error->message);
		  g_error_free (error);
		  return FALSE;
		}
	      return TRUE;
	  }
	  break;

	default: { g_log (G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s: Unknown MPRIS method id %d", G_STRLOC, method_id); return FALSE; }
      }
}
#endif
