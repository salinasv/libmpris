//
// LibMPRIS (C) 2007 deadchip, mirsal
//

#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

#define MPRIS_ROOT_PATH         "/"
#define MPRIS_BUS_NAME_PREFIX   "org.mpris."
#define MPRIS_FDO_IFACE_NAME    "org.freedesktop.MediaPlayer"

DBusConnection * conn = NULL;

struct list_head players;
LIST_HEAD (players);

static char**
demarshal_strv (DBusMessageIter	* iter)
{
  DBusMessageIter iter2;
  char **ret;
  int current_type;
  int len;
  int i;

  current_type = dbus_message_iter_get_arg_type (iter);
  if (current_type != DBUS_TYPE_ARRAY)
    return NULL;

  dbus_message_iter_recurse (iter, &iter2);
  current_type = dbus_message_iter_get_arg_type (&iter2);

  if (current_type != DBUS_TYPE_INVALID && current_type != DBUS_TYPE_STRING)
    return NULL;

  len = dbus_message_iter_get_array_len (&iter2);
  ret = malloc (sizeof (char *) * (len + 1));
  
  i = 0;
  while ((current_type = dbus_message_iter_get_arg_type (&iter2)) != DBUS_TYPE_INVALID)
  {
    const char * aux;
    dbus_message_iter_get_basic (&iter2, &aux); 
    ret[i] = strdup (aux);
    dbus_message_iter_next (&iter2);
    i++;
  }
  ret[i] = NULL; /* terminate StrV with NULL */
  return ret;
}

int
mpris_dbus_init (void)
{
  DBusError err;
  dbus_error_init (&err);

  conn = dbus_bus_get (DBUS_BUS_SESSION, &err);
  return (conn != NULL);
}

MPRISPlayerInfo*
mpris_dbus_get_player_info (const char * player)
{
  MPRISPlayerInfo   * p_info = malloc (sizeof(MPRISPlayerInfo));
  DBusMessage	      * in = NULL, * out = NULL; /* in as in "into DBus" 
                                                    and out as in "from DBus" */

  char* name = (char*) malloc (strlen ("org.mpris.") + strlen (player));
  sprintf (name, "org.mpris.%s", player);

  DBusError err;
  dbus_error_init (&err);

  in = dbus_message_new_method_call (name, MPRIS_ROOT_PATH, 
                  MPRIS_FDO_IFACE_NAME, "Identity");
  out = dbus_connection_send_with_reply_and_block (conn, in, 
                  500 /* let's wait half a second max */, &err);
    
  if ((out == NULL) || dbus_error_is_set (&err))
  {
    fprintf (stderr, "%s:%d: Message call failed!\n", __FILE__, __LINE__);
    free (name);
    return NULL;
  }

  if (!dbus_message_get_args (out, &err, DBUS_TYPE_STRING, 
                          &p_info->name, DBUS_TYPE_INVALID))
  {
    fprintf (stderr, "%s:%d: Couldn't get args from message!\n", 
                    __FILE__, __LINE__);
    free (name);
    return NULL;
  }
  free (name);

  p_info->suffix = strdup (player); 

  dbus_message_unref (in);   
  dbus_message_unref (out);   

  return p_info;
}

struct list_head*
mpris_dbus_list_players (void)
{

  DBusMessage	 * in = NULL, * out = NULL; /* in as in "into DBus" and out as in "from DBus" */

  DBusError err;
  dbus_error_init (&err);

  in = dbus_message_new_method_call ("org.freedesktop.DBus", 
                  "/org/freedesktop/DBus", "org.freedesktop.DBus", "ListNames");

  out = dbus_connection_send_with_reply_and_block (conn, in, 
                  500 /* let's wait half a second max */, &err);
  dbus_message_unref (in);   
    
  if ((out == NULL) || dbus_error_is_set (&err))
  {
    fprintf (stderr, "%s:%d: Message call failed!\n", __FILE__, __LINE__);
    return NULL;
  }


  DBusMessageIter iter;
  if (!dbus_message_iter_init (out, &iter))
  {
    fprintf (stderr, "%s:%d: Couldn't init message iter!\n", __FILE__, __LINE__);
    return NULL;
  }

  char ** names = NULL;
  if (DBUS_TYPE_ARRAY == dbus_message_iter_get_arg_type (&iter)) 
  {
    names = demarshal_strv (&iter);
  }
  else
  {
    dbus_message_unref (out);   
    return NULL;
  }

  int n = 0;
  while (names[n])
  {
    if (!strncasecmp (MPRIS_BUS_NAME_PREFIX, names[n], strlen (MPRIS_BUS_NAME_PREFIX)))
	  {
	    MPRISPlayerInfo * p_info = NULL;
	    char * rstring = rindex (names[n], '.');
	    rstring++;
	    p_info = mpris_dbus_get_player_info (rstring);
	    list_add_tail (&p_info->node, &players);
	  }
    ++n;
  }

  dbus_message_unref (out);   
  return &players;
}
