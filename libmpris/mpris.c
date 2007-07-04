//
// LibMPRIS (C) 2007 deadchip, mirsal
//

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/list.h>

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

void
mpris_metadata_free (MPRISMetadata* metadata)
{
        if (metadata->title) free (metadata->title);
        if (metadata->artist) free (metadata->artist);
        if (metadata->album) free (metadata->album);
        free (metadata);
}

MPRISPlayer*
mpris_player_new (const char * p_id)
{
  MPRISPlayer * player = malloc (sizeof(MPRISPlayer));
  memset (player, 0x00, sizeof(MPRISPlayer));
  
  MPRISPlayerInfo * p_info = mpris_dbus_get_player_info (p_id);

  if (!p_info)
  {
    free (player);
    return NULL;
  }

  player->p_info = p_info; 
  player->listen_thread = (pthread_t*) malloc (sizeof (pthread_t));
  player->lock = (pthread_mutex_t*) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (player->lock, NULL);
  player->callback_functions = 
          (MPRISCallbackFuncs*) malloc (sizeof (MPRISCallbackFuncs));
  return player;
}

void
mpris_player_free (MPRISPlayer * player)
{
  /* TODO: If we keep more stuff in here than just strings, disconnect, etc... */

  if (player->listen_thread) free (player->listen_thread);
  
  if (player->lock) 
  {
          pthread_mutex_destroy (player->lock);
          free (player->lock);
  }

  if (player->p_info->suffix) free (player->p_info->suffix);
  if (player->p_info->name) free (player->p_info->name);
  if (player->p_info) free (player->p_info);
  if (player->callback_functions) free (player->callback_functions);
  if (player) free (player);
}

static void
fill_info_array (MPRISList* item, void* user_data, int pos)
{
        MPRISPlayerInfo** info_array = (MPRISPlayerInfo**) user_data;
        info_array[pos] = (MPRISPlayerInfo*) item->data;
} 

MPRISPlayerInfo**
mpris_list_players (void)
{
  MPRISList *head;
  MPRISPlayerInfo** list;
  MPRISPlayerInfo* item;
  int list_size, last_pos = 0;

  head = mpris_dbus_list_players ();
  if ((list_size = mpris_list_size (head)) == 0)
          return NULL;
  list = malloc ((sizeof (MPRISPlayerInfo*) * list_size) + 1);
  last_pos = mpris_list_foreach (head, fill_info_array, (void*) list);
  list[last_pos] = NULL; /* terminate list with NULL */
  return list;
}
