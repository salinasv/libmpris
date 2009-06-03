/*****************************************************************************
 * mpris-demo.c
 *****************************************************************************
 * LibMPRIS - Copyright (C) 2007 Milosz Derezynski, Mirsal Ennaime
 *
 * Authors:    Milosz Derezynski
 *             Mirsal Ennaime <mirsal dot ennaime at gmail dot com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
//
// LibMPRIS Demo (C) 2007 deadchip, mirsal
//

#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>

static void
cb_StatusChange (MPRISPlayerStatus* status,
                 MPRISPlayer* player, void* user_data)
{
        const char* states[] = { "Playing", "Paused", "Stopped" };
        const char* boolvalue[] = { "Disabled", "Enabled" };
        printf ("Status changed !\n"
                "New status is:\n"
                "State: %s\n"
                "Random: %s\n"
                "Repeat: %s\n"
                "Loop: %s\n",
                states[status->state],
                boolvalue[status->random],
                boolvalue[status->repeat],
                boolvalue[status->loop]);
}

static void
cb_TrackChange (MPRISMetadata* metadata, MPRISPlayer* player, void* user_data)
{
        printf ("Changed Track !\n"
                "New metadata is:\n"
                "Title: %s\n"
                "Artist: %s\n"
                "Album: %s\n",
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
                printf ("Player: '%s' with identity: '%s' implements mpris version %d.%d\n",
                                p_list[i]->suffix, p_list[i]->name,
				p_list[i]->mpris_version.major,
				p_list[i]->mpris_version.minor);
                ++i;
        }

        free (p_list);
        return 0;
}
int
main (int n_args, char **args)
{
  MPRISPlayer * player;
  MPRISMetadata * metadata;
  mpris_client_init ();

  if (n_args != 2)
  {
          list_clients ();
          return EXIT_SUCCESS;
  }

  player = mpris_player_new (args[1]);

  if (!player)
  {
          fprintf (stdout, "Player 'org.mpris.%s' not found on the bus\n",
                          args[1]);
          return EXIT_FAILURE;
  }

  player->callback_functions->track_change = cb_TrackChange;
  player->callback_functions->status_change = cb_StatusChange;

  fprintf (stdout, "MPRIS identity of 'org.mpris.%s' is: '%s'\n",
                  args[1], MPRIS_PLAYER_IDENTITY(player));

  metadata = mpris_metadata_get_current_track(player);
  printf("Current track: %s (%s) %s\n", metadata->artist, metadata->album,
		  metadata->title);
  mpris_metadata_free(metadata);

  mpris_player_start_listen (player);
  pthread_join (*player->listen_thread, NULL);

  mpris_player_free (player);

  return EXIT_SUCCESS;
}
