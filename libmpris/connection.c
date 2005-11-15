#include <stdio.h>
#include <stdlib.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

static char* method_names[] = {
      "PlayNext",
      "PlayPrev",
      "PlayPause",
      "PlayStop",
      "PlayCurrent"
};

int
mpris_invoke_method (MPRISPlayerInfo *p_info, MPRISMethodId method_id)
{
     switch (method_id) 
      {
	case MPRIS_METHOD_PLAY_NEXT:
	case MPRIS_METHOD_PLAY_PREV:
	case MPRIS_METHOD_PLAY_PAUSE:
	case MPRIS_METHOD_PLAY_STOP:
	case MPRIS_METHOD_PLAY_CURRENT:
	  {
	      DBusMessage	   *msg;

	      dbus_message_call_simple (conn,
					&msg,
					p_info->interface,	
					p_info->path, 
					p_info->interface,	
					method_names[method_id]);

	      return 1;
	  }
	  break;

	default: { fprintf (stderr, "%s(%d): Unknown MPRIS method id %d", __FILE__, __LINE__ , method_id); return 0; }
      }
}
