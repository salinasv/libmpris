//
// LibMPRIS (C) 2007 deadchip, mirsal
//

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

int
mpris_client_init (void)
{
  return mpris_dbus_init ();
}

int
mpris_server_init (void)
{
  return mpris_dbus_init ();
}

MPRISPlayer*
mpris_player_new (const char * p_id)
{
  MPRISPlayer * player = malloc (sizeof(MPRISPlayer));
  memset (player, 0x00, sizeof(MPRISPlayer));

  player->p_info = mpris_dbus_get_player_info (p_id);
  return player;
}

void
mpris_player_free (MPRISPlayer * player)
{
  /* TODO: If we keep more stuff in here than just strings, disconnect, etc... */

  /*
  free (player->p_info->suffix);
  free (player->p_info->name);
  free (player->p_info);
  free (player);
  */
}

MPRISPlayerInfo**
mpris_list_players (void)
{
  struct list_head*head;
  MPRISPlayerInfo**list;
  MPRISPlayerInfo*item;

  int n = 0;
  head = mpris_dbus_list_players ();

  list_for_each_entry (item, head, node)
  {
    n++;
  }

  list = malloc ((sizeof (MPRISPlayerInfo*) * n) + 1);
  n = 0;
  list_for_each_entry(item, head, node)
  {
    list[n++] = item;
  }
  list[n] = NULL; /* terminate list with NULL */
  return list;
}
