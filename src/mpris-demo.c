#include <stdlib.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>

void
demo_set_stream_pos (MPRISPlayer *player, int32_t stream_pos)
{
    int32_t volume;
    mpris_player_invoke_method (player, MPRIS_METHOD_GET_VOLUME, &volume);

    fprintf (stdout, "StreamPos: %d\n", stream_pos);
    fprintf (stdout, "Volume: %d\n", volume);
}

void
demo_set_playstatus (MPRISPlayer *player, int32_t playstatus)
{
    fprintf (stdout, "Playstatus: %d\n", playstatus);
}

int
main (int n_args, char **args)
{
  MPRISPlayer *player;
  int count = 0;

  mpris_client_init ();

  player = mpris_player_new ("bmpx",
			     demo_set_stream_pos,
			     NULL,
			     demo_set_playstatus,
			     NULL);


  mpris_player_start_listen (player);

  while (count < 1000)
    {
	usleep (5000);
	count++;
    }

  mpris_player_stop_listen (player);
  fprintf (stdout, "Exit");
  usleep (2000000);

  exit (0);
}
