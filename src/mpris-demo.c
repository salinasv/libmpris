//
// LibMPRIS Demo (C) 2007 deadchip, mirsal
//

#include <stdlib.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>

int
main (int n_args, char **args)
{
  mpris_client_init ();

  MPRISPlayer * player;
  player = mpris_player_new (args[1]);

  fprintf (stdout, "MPRIS identity of 'org.mpris.%s' is: '%s'\n", args[1], MPRIS_PLAYER_NAME(player)); 
 
  mpris_player_free (player);

  return EXIT_SUCCESS;
}
