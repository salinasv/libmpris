#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

#define MPRIS_MATCH_RULE \
	"type='signal',interface='org.freedesktop.MediaPlayer'"

static void*
listen_thread (void* user_data);

static DBusHandlerResult
handle_signals (DBusConnection* conn, DBusMessage* msg, void* user_data);

int
mpris_player_invoke_method (MPRISPlayer *player, MPRISMethodId method_id, ...) 
{
}

static void*
listen_thread (void* user_data)
{
	MPRISPlayer* player = (MPRISPlayer*) user_data;
	while (!player->thread_exit)
		dbus_connection_read_write_dispatch (conn, -1);
}

int
mpris_player_start_listen (MPRISPlayer *player)
{
        DBusError err;

        dbus_error_init (&err);

	dbus_threads_init_default ();
	pthread_mutex_init (player->lock, NULL);
	player->thread_exit = 0;
        dbus_bus_add_match (conn, MPRIS_MATCH_RULE, &err);
        dbus_connection_add_filter (conn,
                 handle_signals, (void*) player, NULL);
	pthread_create (player->listen_thread, 
			NULL, listen_thread, (void*) player);
	dbus_error_free (&err);

        return 0;
}

void
mpris_player_stop_listen (MPRISPlayer *player)
{
	DBusError err;

	dbus_error_init (&err);
	player->thread_exit = 1;
	dbus_connection_remove_filter (conn, handle_signals, (void*) player);
	dbus_bus_remove_match (conn, MPRIS_MATCH_RULE, &err);
	pthread_mutex_destroy (player->lock);
	pthread_join (*player->listen_thread, NULL);
	dbus_error_free (&err);
}

static DBusHandlerResult
handle_signals (DBusConnection* conn, DBusMessage* msg, void* user_data)
{
	MPRISPlayer *player = (MPRISPlayer*) user_data;

	if (dbus_message_is_signal (msg, 
				"org.freedesktop.MediaPlayer", "TrackChange"))
                 printf ("Changed Track !\n");
	return 0;
}
