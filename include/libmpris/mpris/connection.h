#ifndef _MPRIS_CONNECTION_H_
#define _MPRIS_CONNECTION_H_

#include <mpris/mpris.h>
#include <mpris/dbus.h>

typedef enum { 
      MPRIS_METHOD_PLAY_NEXT,
      MPRIS_METHOD_PLAY_PREV,
      MPRIS_METHOD_PLAY_PAUSE,
      MPRIS_METHOD_PLAY_STOP,
      MPRIS_METHOD_PLAY_CURRENT,

      MPRIS_METHOD_GET_CURRENT_TITLE,
      MPRIS_METHOD_GET_CURRENT_TRACK,

      MPRIS_METHOD_GET_VOLUME,
      MPRIS_METHOD_SET_VOLUME

/*
      MPRIS_METHOD_GET_CURRENT_URI,
      MPRIS_METHOD_GET_METADATA_FOR_URI
*/
} MPRISMethodId;

int
mpris_client_invoke_method (MPRISClient *client, MPRISMethodId method_id, void **data);

int
mpris_client_start_listen (MPRISClient *client);

void
mpris_client_stop_listen (MPRISClient *client);
 
#endif /* _MPRIS_CONNECTION_H_ */
