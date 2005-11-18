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

int
main (int n_args, char **args)
{
  MPRISClient *client;

  mpris_init ();

  client = mpris_client_new ("bmpx",
			     demo_set_stream_pos,
			     NULL,
			     NULL,
			     NULL);

  mpris_client_init_signals (client);

  exit (0);
}
