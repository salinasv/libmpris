#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <mpris/mpris-list.h>

typedef struct _MPRISPlayerInfo
{
  struct list_head node;

  char *suffix;
  char *name;

} MPRISPlayerInfo;

typedef struct _MPRISPlayer
{
  MPRISPlayerInfo * p_info;
  void            * user_data;

} MPRISPlayer;

#define MPRIS_PLAYER_NAME(x) (x->p_info->name)

int
mpris_client_init   (void);

int
mpris_server_init   (void);

MPRISPlayer*
mpris_player_new    (const char *player);

void
mpris_player_free   (MPRISPlayer* player);

MPRISPlayerInfo**
mpris_list_players  (void);

#endif /* _MPRIS_H_ */
