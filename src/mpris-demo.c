#include <stdlib.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>
#include <mpris/mpris-list.h>

void
demo_set_stream_pos (MPRISClient *client, int32_t stream_pos)
{
    fprintf (stdout, "StreamPos: %d\n", stream_pos);
}

void
demo_set_playstatus (MPRISClient *client, int32_t playstatus)
{
    fprintf (stdout, "Playstatus: %d\n", playstatus);
}

int
main (int n_args, char **args)
{
  MPRISClient *client;

  mpris_init ();

  client = mpris_client_new ("bmpx",
			     demo_set_stream_pos,
			     NULL,
			     demo_set_playstatus,
			     NULL);

  mpris_client_start_listen (client);

  while (1)
    {
	usleep (500);
    }

  exit (0);
}
