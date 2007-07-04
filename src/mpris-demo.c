//
// LibMPRIS Demo (C) 2007 deadchip, mirsal
//

#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

static void
cb_TrackChange (MPRISMetadata* metadata, MPRISPlayer* player, void* user_data)
{
        printf ("Changed Track !\n"
                "New metadata is:\n"
                "Title:%s\n"
                "Artist:%s\n"
                "Album%s\n", 
                metadata->title, 
                metadata->artist, 
                metadata->album);
}

static int
list_clients (void)
{
        MPRISPlayerInfo** p_list = NULL;
        int i = 0;
        if (!(p_list = mpris_list_players ()))
                return EXIT_FAILURE;
        while (p_list[i])
        {
                printf ("Player: '%s' with identity: '%s'\n", 
                                p_list[i]->suffix, p_list[i]->name);
                ++i;
        }

        free (p_list);
        return 0;
}
int
main (int n_args, char **args)
{
  MPRISPlayer * player;
  mpris_client_init ();

  if (n_args != 2)
  {
          list_clients ();
          return EXIT_SUCCESS;
  }

  player = mpris_player_new (args[1]);
  player->callback_functions->track_change = cb_TrackChange;

  fprintf (stdout, "MPRIS identity of 'org.mpris.%s' is: '%s'\n", 
                  args[1], MPRIS_PLAYER_NAME(player)); 
  mpris_player_start_listen (player); 
  pthread_join (*player->listen_thread, NULL);

  mpris_player_free (player);

  return EXIT_SUCCESS;
}
