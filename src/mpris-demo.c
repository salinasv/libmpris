//
// LibMPRIS Demo (C) 2007 deadchip, mirsal
//

#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>

int
main (int n_args, char **args)
{
  MPRISPlayer * player;
  mpris_client_init ();

  if (n_args != 2)
	  return EXIT_FAILURE;
  player = mpris_player_new (args[1]);

  fprintf (stdout, "MPRIS identity of 'org.mpris.%s' is: '%s'\n", args[1], MPRIS_PLAYER_NAME(player)); 
  mpris_player_start_listen (player); 
  pthread_join (*player->listen_thread, NULL);

  mpris_player_free (player);

  return EXIT_SUCCESS;
}
