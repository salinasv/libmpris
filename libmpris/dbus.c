#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

#define MPRIS_INTERFACE_PREFIX	"org.mpris"

DBusConnection  *conn;

struct list_head players;

LIST_HEAD (players);

static char**
demarshal_strv (DBusMessageIter	        *iter)
{
  DBusMessageIter subiter;
  char **ret;
  int current_type;
  int len;
  int i;

  current_type = dbus_message_iter_get_arg_type (iter);
  if (current_type != DBUS_TYPE_ARRAY)
      return NULL;

  dbus_message_iter_recurse (iter, &subiter);

  current_type = dbus_message_iter_get_arg_type (&subiter);
  if (current_type != DBUS_TYPE_INVALID
      && current_type != DBUS_TYPE_STRING)
      return NULL;

  len = dbus_message_iter_get_array_len (&subiter);
  ret = malloc (sizeof (char *) * (len + 1));
  
  i = 0;
  while ((current_type = dbus_message_iter_get_arg_type (&subiter)) != DBUS_TYPE_INVALID)
    {
      dbus_message_iter_get_basic (&subiter, &(ret[i]));
      ret[i] = strndup (ret[i], strlen(ret[i]));

      dbus_message_iter_next (&subiter);
      i++;
    }
  ret[i] = NULL; 
  
  return ret;
}

static int
dbus_message_call_simple (DBusConnection *conn,
			  DBusMessage **msg,
			  const char *target,
			  const char *object,
			  const char *iface,
			  const char *method)
{
  DBusPendingCall  *pending;

  *msg = dbus_message_new_method_call(target,
				     object,  
				     iface,
			             method);

  if (!*msg)
    return 0;

  if (!dbus_connection_send_with_reply (conn, *msg, &pending, -1)) // -1 is default timeout
    return 0;

  if (!pending)
    return 0;

  dbus_connection_flush(conn);
  dbus_message_unref(*msg);
  dbus_pending_call_block(pending);
  *msg = dbus_pending_call_steal_reply(pending);

  if (!*msg)
    return 0;

  dbus_pending_call_unref(pending);

  return 1;
}

struct list_head*
mpris_dbus_list (void)
{
  DBusError	    err;
  DBusMessage	   *msg;
  DBusMessageIter   args;
  char		  **names = NULL;
  int		    n = 0;

  dbus_error_init (&err);
  conn = dbus_bus_get (DBUS_BUS_SESSION, &err);

  if (!conn)
      return NULL;

  dbus_message_call_simple (conn,
			    &msg,
			    "org.freedesktop.DBus",
			    "/org/freedesktop/DBus",
			    "org.freedesktop.DBus",
			    "ListNames");

  if (!dbus_message_iter_init(msg, &args))
      return NULL;

  else if (DBUS_TYPE_ARRAY == dbus_message_iter_get_arg_type(&args)) 
      names = demarshal_strv (&args);
  else
    {
      dbus_message_unref(msg);   
      return NULL;
    }

  dbus_message_unref(msg);   

  while (names[n])
    {
      if (!strncasecmp (MPRIS_INTERFACE_PREFIX, names[n], strlen(MPRIS_INTERFACE_PREFIX)))
	{
	  MPRISPlayerInfo  *p_info = malloc (sizeof(MPRISPlayerInfo));
	  char		   *path,
			   *rstring;

#define OBJ_PREFIX "/org/mpris/"

	  rstring = rindex (names[n], '.');
	  rstring++;

	  path = malloc (strlen(rstring) + strlen("/org/mpris//SystemControl")+1);
	  memcpy (path, OBJ_PREFIX, strlen(OBJ_PREFIX)); 
	  memcpy (path+strlen(OBJ_PREFIX), rstring, strlen(rstring));
	  memcpy (path+strlen(OBJ_PREFIX)+strlen(rstring), "/SystemControl", strlen("/SystemControl")+1);

	  dbus_message_call_simple (conn,
			    &msg,
			    names[n], 
			    path, 
			    names[n], 
			    "Identity");

	  if (dbus_message_iter_init(msg, &args))
	    {
	      dbus_message_iter_get_basic (&args, &(p_info->name));
	    }
	  else
	    {
	      fprintf (stderr, "Couldn't acquire MPRIS remote Identity!");
	      p_info->name = NULL;
	    }

	  dbus_message_unref(msg);   

	  p_info->interface = strndup (names[n], strlen(path));
	  p_info->path = strndup (path, strlen(path));

	  free (path);

	  list_add_tail (&p_info->node, &players);
	}
      n++;
    }

  return &players;
}
