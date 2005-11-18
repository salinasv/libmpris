#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void*
dbus_msg_poll_thread (MPRISPlayer *player)
{
  DBusMessage	   *msg;
  DBusMessageIter   args;
  void		   *sigvalue;

  while (1)
    {
      if (player->t_exit == 1)
	pthread_exit (NULL);


      dbus_connection_read_write(conn, 0);

      // non blocking read of the next available message
      while (NULL != (msg = dbus_connection_pop_message(conn)))
      {
	// check if the message is a signal from the correct interface and with the correct name
	if (dbus_message_is_signal(msg, player->p_info->interface, "SetPlaystatus"))
	{
	  // read the parameters
	 if (!dbus_message_iter_init(msg, &args))
            fprintf(stderr, "Message has no arguments!\n"); 
         else if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&args)) 
            dbus_message_iter_get_basic(&args, &sigvalue);

	 if (player->cb_set_playstatus)
	      player->cb_set_playstatus (player, (int32_t)sigvalue);

	 continue;
	}


      // check if the message is a signal from the correct interface and with the correct name
      if (dbus_message_is_signal(msg, player->p_info->interface, "SetStreamPos"))
	{
	  // read the parameters
	  if (!dbus_message_iter_init(msg, &args))
            fprintf(stderr, "Message has no arguments!\n"); 
         else if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&args)) 
            dbus_message_iter_get_basic(&args, &sigvalue);

	 if (player->cb_set_stream_pos)
	      player->cb_set_stream_pos (player, (int32_t)sigvalue);

	 continue;
	}

      if (dbus_message_is_signal(msg, player->p_info->interface, "SetVolume"))
	{
	  // read the parameters
	  if (!dbus_message_iter_init(msg, &args))
            fprintf(stderr, "Message has no arguments!\n"); 
         else if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&args)) 
            dbus_message_iter_get_basic(&args, &sigvalue);

	 if (player->cb_set_volume)
	      player->cb_set_volume (player, (int32_t)sigvalue);

	 continue;
	}

      // check if the message is a signal from the correct interface and with the correct name
      if (dbus_message_is_signal(msg, player->p_info->interface, "TrackChange"))
	{
         printf("TrackChange\n");

	 if (player->cb_track_change)
	      player->cb_track_change (player);

	 continue;
	}

      // free the message
      dbus_message_unref(msg);
    }
  }
}

void
mpris_player_stop_listen (MPRISPlayer *player)
{
    player->t_exit = 1;

    //Really needed? Prolly just nonsense
    usleep (250);
}
 
int
mpris_player_start_listen (MPRISPlayer *player)
{
  DBusError	    err;
  char		   *buf;
  size_t	    size;
  const char	   *signal = "signal";
  pthread_t	    thread;

#define _EXT "type='',interface=''"

  size = strlen(signal)+strlen(player->p_info->interface)+strlen(_EXT)+1;
  buf = malloc(size);
  sprintf (buf, "type='%s',interface='%s'", signal, player->p_info->interface); 

  dbus_error_init (&err);
  dbus_bus_add_match(conn, buf, &err); 

  dbus_connection_flush(conn);

  if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Match Error (%s)\n", err.message);
      return 0;
  }

  pthread_create (&thread, NULL, dbus_msg_poll_thread, player);

  return 1;
}


int
mpris_player_invoke_method (MPRISPlayer *player, MPRISMethodId method_id, ...) 
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

	      msg = dbus_message_new_method_call (player->p_info->interface,	
					    player->p_info->path, 
					    player->p_info->interface,	
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

	      return 1;
	  }
	  break;

	case MPRIS_METHOD_GET_CURRENT_TRACK:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;
	      void		   *data;

	      msg = dbus_message_new_method_call
					(player->p_info->interface,	
					 player->p_info->path, 
					 player->p_info->interface,	
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

	     dbus_message_iter_get_basic(&args, &data);

	     /* free reply and close connection*/
	     dbus_message_unref(msg);   
	     return 1;
	  }
	  break;


	case MPRIS_METHOD_GET_CURRENT_TITLE:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;
	      void		   *data;
	      char		  **title;
	      va_list		    ap;

	      msg = dbus_message_new_method_call
					(player->p_info->interface,	
					 player->p_info->path, 
					 player->p_info->interface,	
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

	     dbus_message_iter_get_basic(&args, &data);
	     va_start (ap, method_id);
	     title = va_arg (ap, char**);  
	     *title = data;
	     va_end (ap);

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
	      void		   *data;
	      int32_t		   *volume;
	      va_list		    ap;

	      msg = dbus_message_new_method_call
					(player->p_info->interface,	
					 player->p_info->path, 
					 player->p_info->interface,	
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
		  return 0;
		}

	      dbus_message_iter_get_basic (&args, &data); 
	      dbus_message_unref(msg);

	      va_start (ap, method_id);
	      volume = va_arg (ap, int32_t*);  
	      *volume = (int32_t)data;
	      va_end (ap);

	      return 1;
	  }
	  break;

	case MPRIS_METHOD_SET_VOLUME:
	  {
	      DBusMessage	   *msg;
	      DBusMessageIter	    args;
	      DBusPendingCall	   *pending;
	      int32_t		    volume;
	      va_list		    ap;

	      va_start (ap, method_id);
	      volume = va_arg (ap, int32_t);  
	      va_end (ap);

	      msg = dbus_message_new_method_call (player->p_info->interface,	
					    player->p_info->path, 
					    player->p_info->interface,	
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
