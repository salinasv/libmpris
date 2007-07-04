#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <pthread.h>
#include <mpris/types.h>

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
