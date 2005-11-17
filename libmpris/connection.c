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
      "PlayCurrent",
      "GetCurrentTitle",
      "GetCurrentTrack",
      "VolumeGet",
      "VolumeSet"
};

int
mpris_invoke_method (MPRISPlayerInfo *p_info, MPRISMethodId method_id, void **data)
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
	      DBusPendingCall	   *pending;

	      msg = dbus_message_new_method_call (p_info->interface,	
					    p_info->path, 
					    p_info->interface,	
					    method_names[method_id]);

	      /* send message and get a handle for a reply*/
	     if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) /* -1 is default timeout*/
	       {
		  fprintf(stderr, "Out Of Memory!\n"); 
		  return 0;
	       }

	     if (NULL == pending)
	       { 
		  fprintf(stderr, "Pending Call Null\n"); 
		  return 0;
	       }

	      /* free message*/
	      dbus_message_unref(msg);
	      dbus_connection_flush (conn);

	      *data = NULL;
	      return 1;
	  }
	  break;

	case MPRIS_METHOD_GET_CURRENT_TRACK:
	case MPRIS_METHOD_GET_CURRENT_TITLE:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;

	      msg = dbus_message_new_method_call
					(p_info->interface,	
					 p_info->path, 
					 p_info->interface,	
					 method_names[method_id]); 


	      if (NULL == msg) { 
		fprintf(stderr, "Message Null\n");
		return 0;
	      }

	      /* send message and get a handle for a reply*/
	      if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { /* -1 is default timeout*/
		fprintf(stderr, "Out Of Memory!\n"); 
		return 0;
	      }
	      if (NULL == pending) { 
		fprintf(stderr, "Pending Call Null\n"); 
		return 0; 
	      }
	      dbus_connection_flush(conn);

	      /* free message*/
	      dbus_message_unref(msg);
        
	      /* block until we receive a reply*/
	      dbus_pending_call_block(pending);
   
	      /* get the reply message*/
	      msg = dbus_pending_call_steal_reply(pending);
              if (NULL == msg) {
	        fprintf(stderr, "Reply Null\n"); 
		return 0; 
	      }
	      /* free the pending message handle*/
              dbus_pending_call_unref(pending);

	      /* read the parameters*/
              if (!dbus_message_iter_init(msg, &args)) {
		  fprintf(stderr, "Message has no arguments!\n"); 
		  return 0;
              }

	     dbus_message_iter_get_basic(&args, data);

	     /* free reply and close connection*/
	     dbus_message_unref(msg);   

	     return 1;
	  }
	  break;

	case MPRIS_METHOD_GET_VOLUME:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;

	      msg = dbus_message_new_method_call
					(p_info->interface,	
					 p_info->path, 
					 p_info->interface,	
					 method_names[method_id]); 


	      /* send message and get a handle for a reply*/
	     if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) /* -1 is default timeout*/
	       {
		  fprintf(stderr, "Out Of Memory!\n"); 
		  return 0;
	       }

	     if (NULL == pending)
	       { 
		  fprintf(stderr, "Pending Call Null\n"); 
		  return 0;
	       }
	      dbus_connection_flush (conn);
	      dbus_message_unref(msg);

	      /* block until we receive a reply*/
	      dbus_pending_call_block(pending);
   
	      /* get the reply message*/
	      msg = dbus_pending_call_steal_reply(pending);
	      if (NULL == msg)
		{
		  fprintf(stderr, "Reply Null\n"); 
		  return 0;
		}
	      /* free the pending message handle*/
	      dbus_pending_call_unref(pending);

	     if (!dbus_message_iter_init(msg, &args))
	       {
		  *data = NULL; 
		  return 0;
		}

	      dbus_message_iter_get_basic (&args, data); 
	      dbus_message_unref(msg);

	      return 1;
	  }
	  break;

	case MPRIS_METHOD_SET_VOLUME:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;
	      int16_t		    volume = (int16_t)*data;

	      fprintf (stderr,"volume: %d\n",volume);

	      msg = dbus_message_new_method_call (p_info->interface,	
					    p_info->path, 
					    p_info->interface,	
					    method_names[method_id]); 

	     dbus_message_iter_init_append(msg, &args);
	     if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT16, &volume))
	      {
		 fprintf(stderr, "Unable to append data!\n");
		 return 0;
	      }

	      /* send message and get a handle for a reply*/
	     if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) /* -1 is default timeout*/
	       {
		  fprintf(stderr, "Out Of Memory!\n"); 
		  return 0;
	       }

	     if (NULL == pending)
	       { 
		  fprintf(stderr, "Pending Call Null\n"); 
		  return 0;
	       }

	      /* free message*/
	      dbus_message_unref(msg);
	      dbus_connection_flush (conn);

	    return 1;

	  }
	  break;

	default: { fprintf (stderr, "%s(%d): Unknown MPRIS method id %d", __FILE__, __LINE__ , method_id); return 0; }
      }
}
