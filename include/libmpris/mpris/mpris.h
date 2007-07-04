#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <pthread.h>
#include <mpris/types.h>

#define MPRIS_PLAYER_IDENTITY(player) \
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

int
mpris_player_invoke_method (MPRISPlayer *player, MPRISMethodId method_id, ...); 

int
mpris_player_start_listen (MPRISPlayer *player);

void
mpris_player_stop_listen (MPRISPlayer *player);
 
#endif /* _MPRIS_H_ */
