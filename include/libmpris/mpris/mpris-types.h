#ifndef _MPRIS_TYPES_H_
#define _MPRIS_TYPES_H_

#include <pthread.h>
#include <mpris/list.h>
#include <string.h>

typedef struct _MPRISCallbackFuncs MPRISCallbackFuncs;

typedef struct _MPRISPlayerInfo
{
  MPRISList * node;

  char *suffix;
  char *name;

} MPRISPlayerInfo;

typedef struct _MPRISPlayer
{
  MPRISPlayerInfo       * p_info;
  pthread_mutex_t       * lock;
  pthread_t             * listen_thread;
  MPRISCallbackFuncs    * callback_functions;
  int                     thread_exit;
  void                  * user_data;

} MPRISPlayer;

typedef struct _MPRISMetadata
{
        char* title;
        char* artist;
        char* album;
} MPRISMetadata;

typedef void (* MPRISCallbackTrackChange) (MPRISMetadata*, MPRISPlayer*, void*);
typedef void (* MPRISCallbackStatusChange) (int, MPRISPlayer, void*);
typedef void (* MPRISCallbackCapsChange) (int, MPRISPlayer, void*);

struct _MPRISCallbackFuncs
{
        MPRISCallbackTrackChange  track_change;
        MPRISCallbackStatusChange status_change;
        MPRISCallbackCapsChange   caps_change;
};

#define MPRIS_PLAYER_NAME(player) \
        player->p_info ? (player->p_info->name) : "NOT CONNECTED"
#define MPRIS_PLAYER_LOCK(player) pthread_mutex_lock (player->lock)
#define MPRIS_PLAYER_UNLOCK(player) pthread_mutex_unlock (player->lock)

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

#endif /* _MPRIS_TYPES_H_ */
