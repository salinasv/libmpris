#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <pthread.h>
#include <mpris/list.h>

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

int
mpris_client_init   (void);

int
mpris_server_init   (void);

void
mpris_metadata_free (MPRISMetadata*);

MPRISPlayer*
mpris_player_new    (const char *player);

void
mpris_player_free   (MPRISPlayer* player);

MPRISPlayerInfo**
mpris_list_players  (void);

#endif /* _MPRIS_H_ */
