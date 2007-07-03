#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

static void*
listen_thread (void* user_data);

static DBusHandlerResult
handle_signals (DBusConnection* conn, DBusMessage* msg, void* user_data);

static char*
build_match_rule (MPRISPlayer* player);

#define SIGNAL_MATCH_RULE_BASE \
	"type='signal',interface='org.freedesktop.MediaPlayer'"

static char*
match_rule_new (MPRISPlayer* player)
{
        char* rule = NULL;
        rule = (char*) malloc (strlen (SIGNAL_MATCH_RULE_BASE) +
                               strlen (",sender='org.mpris.'") + 
                               strlen (player->p_info->suffix) +1);
        sprintf (rule, "%s,sender='org.mpris.%s'", SIGNAL_MATCH_RULE_BASE, 
                        player->p_info->suffix);
        return rule;
}

#undef SIGNAL_MATCH_RULE_BASE

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
        char* match_rule = match_rule_new (player);

        dbus_error_init (&err);

	dbus_threads_init_default ();
	player->thread_exit = 0;
        dbus_bus_add_match (conn, match_rule, &err);
        dbus_connection_add_filter (conn,
                 handle_signals, (void*) player, NULL);
	pthread_create (player->listen_thread, 
			NULL, listen_thread, (void*) player);
	dbus_error_free (&err);
        free (match_rule);

        return 0;
}

void
mpris_player_stop_listen (MPRISPlayer *player)
{
	DBusError err;
        char* match_rule = match_rule_new (player);

	dbus_error_init (&err);
	player->thread_exit = 1;
	dbus_connection_remove_filter (conn, handle_signals, (void*) player);
	dbus_bus_remove_match (conn, match_rule, &err);
	pthread_join (*player->listen_thread, NULL);
	dbus_error_free (&err);
        free (match_rule);
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
