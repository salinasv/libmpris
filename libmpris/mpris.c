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
mpris_player_new (const char *player,
		  MPRIS_CbSetStreamPos  cb_set_stream_pos,
		  MPRIS_CbTrackChange   cb_track_change,
		  MPRIS_CbSetPlaystatus cb_set_playstatus,
		  MPRIS_CbSetVolume	cb_set_volume)
{
  MPRISPlayer *_player = malloc (sizeof(MPRISPlayer));
  memset (_player, 0x00, sizeof(MPRISPlayer));

  _player->p_info = mpris_dbus_get_player_info (player);
  _player->cb_set_stream_pos = cb_set_stream_pos;
  _player->cb_track_change = cb_track_change;
  _player->cb_set_playstatus = cb_set_playstatus;
  _player->cb_set_volume = cb_set_volume;

  _player->t_exit = 0;

  return _player;
}

MPRISPlayerInfo**
mpris_list_players (void)
{
  struct list_head *head;
  MPRISPlayerInfo **list;
  MPRISPlayerInfo *item;
  int count = 0;

  head = mpris_dbus_list_players ();

  list_for_each_entry (item, head, node)
    {
      count++;
    }

  list = malloc ((sizeof (MPRISPlayerInfo*) * count) + 1);
  count = 0;

  list_for_each_entry (item, head, node)
    {
      list[count++] = item;
    }
  list[count] = NULL;
  
  return list;
}

