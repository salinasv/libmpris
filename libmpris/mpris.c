#include <stddef.h>
#include <stdlib.h>
#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

int
mpris_init (void)
{
  return mpris_dbus_init ();
}

MPRISClient*
mpris_client_new (const char *player,
		  MPRIS_CbSetStreamPos  cb_set_stream_pos,
		  MPRIS_CbTrackChange   cb_track_change,
		  MPRIS_CbSetPlaystatus cb_set_playstatus,
		  MPRIS_CbSetVolume	cb_set_volume)
{
  MPRISClient *client = malloc (sizeof(MPRISClient));
  memset (client, 0x00, sizeof(MPRISClient));

  client->p_info = mpris_dbus_get_p_info (player);
  client->cb_set_stream_pos = cb_set_stream_pos;
  client->cb_track_change = cb_track_change;
  client->cb_set_playstatus = cb_set_playstatus;
  client->cb_set_volume = cb_set_volume;

  client->t_exit = 0;

  return client;
}

MPRISPlayerInfo**
mpris_list_clients (void)
{
  struct list_head *head;
  MPRISPlayerInfo **list;
  MPRISPlayerInfo *item;
  int count = 0;

  head = mpris_dbus_list_clients ();

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

