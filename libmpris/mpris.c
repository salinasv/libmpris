/*****************************************************************************
 * mpris.c : libmpris client side dbus wrapper for media players
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
// LibMPRIS (C) 2007 deadchip, mirsal
//

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/list.h>

static void*
listen_thread (void* user_data);

static DBusHandlerResult
handle_signals (DBusConnection* conn, DBusMessage* msg, void* user_data);

static MPRISMetadata*
demarshal_metadata (DBusMessage* msg);

static MPRISPlayerStatus*
demarshal_status (DBusMessage* msg);

#define SIGNAL_MATCH_RULE_BASE \
	"type='signal',interface='org.freedesktop.MediaPlayer'"


int
mpris_client_init (void)
{
	return mpris_dbus_init();
}

int
mpris_server_init (void)
{
	return mpris_dbus_init();
}

void
mpris_metadata_free (MPRISMetadata* metadata)
{
	if (!metadata)
		return;

	free(metadata->location);
	free(metadata->title);
	free(metadata->artist);
	free(metadata->album);
	free(metadata->tracknumber);
	free(metadata->genere);
	free(metadata->comment);
	free(metadata->aturl);

	free (metadata);
}

MPRISPlayer*
mpris_player_new (const char * p_id)
{
	MPRISPlayer * player = calloc(1, sizeof(MPRISPlayer));
	memset(player, 0x00, sizeof(MPRISPlayer));

	MPRISPlayerInfo * p_info = mpris_dbus_get_player_info(p_id);

	if (!p_info) {
		free (player);
		return NULL;
	}

	player->p_info = p_info;
	player->listen_thread = (pthread_t*) malloc(sizeof(pthread_t));
	player->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init (player->lock, NULL);
	player->callback_functions =
		(MPRISCallbackFuncs*) calloc(1, sizeof(MPRISCallbackFuncs));

	return player;
}

void
mpris_player_free (MPRISPlayer * player)
{
	/* TODO: If we keep more stuff in here than just strings, disconnect, etc... */

	if (!player)
		return;

	if (player->lock) {
		pthread_mutex_destroy(player->lock);
		free(player->lock);
	}

	free(player->listen_thread);
	free(player->p_info->suffix);
	free(player->p_info->name);
	free(player->p_info);
	free(player->callback_functions);
	free(player);
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
	int list_size, last_pos = 0;

	head = mpris_dbus_list_players ();

	if ((list_size = mpris_list_size(head)) == 0)
		return NULL;

	list = malloc ((sizeof(MPRISPlayerInfo*) * list_size) + 1);
	last_pos = mpris_list_foreach(head, fill_info_array, (void*) list);
	list[last_pos] = NULL; /* terminate list with NULL */

	return list;
}

static char*
match_rule_new (MPRISPlayer* player)
{
	char* rule = NULL;
	rule = (char*) malloc (strlen(SIGNAL_MATCH_RULE_BASE) +
			strlen(",sender='org.mpris.'") +
			strlen(player->p_info->suffix) +1);
	sprintf(rule, "%s,sender='org.mpris.%s'", SIGNAL_MATCH_RULE_BASE,
			player->p_info->suffix);

	return rule;
}

#undef SIGNAL_MATCH_RULE_BASE

#ifdef GNUC
static void* __attribute__ ((unused))
#else
static void*
#endif
listen_thread (void* user_data)
{
	MPRISPlayer* player = (MPRISPlayer*) user_data;

	while (!player->thread_exit)
		dbus_connection_read_write_dispatch (conn, -1);

#ifndef GNUC
	return NULL; // stupid compilers
#endif
}

int
mpris_player_start_listen (MPRISPlayer *player)
{
	DBusError err;
	char* match_rule = match_rule_new (player);

	dbus_error_init (&err);

	dbus_threads_init_default();
	player->thread_exit = 0;
	dbus_bus_add_match(conn, match_rule, &err);
	dbus_connection_add_filter(conn,
			handle_signals, (void*) player, NULL);
	pthread_create (player->listen_thread,
			NULL, listen_thread, (void*) player);
	dbus_error_free(&err);
	free(match_rule);

	return 0;
}

void
mpris_player_stop_listen (MPRISPlayer *player)
{
	DBusError err;
	char* match_rule = match_rule_new(player);

	dbus_error_init(&err);
	player->thread_exit = 1;
	dbus_connection_remove_filter(conn, handle_signals, (void*) player);
	dbus_bus_remove_match(conn, match_rule, &err);
	pthread_join(*player->listen_thread, NULL);
	dbus_error_free(&err);
	free(match_rule);
}

static void get_meta_item_str(DBusMessageIter *dict_entry, char **item)
{
	DBusMessageIter variant;
	char *str_buf;

	dbus_message_iter_next(dict_entry);
	dbus_message_iter_recurse(dict_entry, &variant);
	dbus_message_iter_get_basic(&variant, (void*) &str_buf);

	*item = malloc(strlen(str_buf) + 1);
	strcpy(*item, str_buf);
}

static void get_meta_item_uint32(DBusMessageIter *dict_entry, uint32_t *item)
{
	DBusMessageIter variant;

	dbus_message_iter_next(dict_entry);
	dbus_message_iter_recurse(dict_entry, &variant);
	dbus_message_iter_get_basic(&variant, (void*) item);
}


static MPRISMetadata*
demarshal_metadata (DBusMessage* msg)
{
	DBusMessageIter args, dict, dict_entry;
	MPRISMetadata* metadata = NULL;
	char* str_buf = NULL;

	metadata = (MPRISMetadata*) calloc(1, sizeof (MPRISMetadata));
	dbus_message_iter_init(msg, &args);
	dbus_message_iter_recurse(&args, &dict);

	do
	{
		dbus_message_iter_recurse(&dict, &dict_entry);
		dbus_message_iter_get_basic(&dict_entry, (void*) &str_buf);

		if (!strcmp(str_buf, "location"))
			get_meta_item_str(&dict_entry, &metadata->location);
		else if (!strcmp(str_buf, "title"))
			get_meta_item_str(&dict_entry, &metadata->title);
		else if (!strcmp(str_buf, "artist"))
			get_meta_item_str(&dict_entry, &metadata->artist);
		else if (!strcmp(str_buf, "album"))
			get_meta_item_str(&dict_entry, &metadata->album);
		else if (!strcmp(str_buf, "tracknumber"))
			get_meta_item_str(&dict_entry, &metadata->tracknumber);
		else if (!strcmp(str_buf, "time"))
			get_meta_item_uint32(&dict_entry, &(metadata->time));
		else if (!strcmp(str_buf, "mtime"))
			get_meta_item_uint32(&dict_entry, &(metadata->mtime));
		else if (!strcmp(str_buf, "genere"))
			get_meta_item_str(&dict_entry, &metadata->genere);
		else if (!strcmp(str_buf, "comment"))
			get_meta_item_str(&dict_entry, &metadata->comment);
		else if (!strcmp(str_buf, "rating"))
			get_meta_item_uint32(&dict_entry, &(metadata->rating));
		else if (!strcmp(str_buf, "year"))
			get_meta_item_uint32(&dict_entry, &(metadata->year));
		else if (!strcmp(str_buf, "date"))
			get_meta_item_uint32(&dict_entry, &(metadata->date));
		else if (!strcmp(str_buf, "aturl"))
			get_meta_item_str(&dict_entry, &metadata->aturl);

	} while (dbus_message_iter_next(&dict));

	return metadata;
}

#define GET_STATUS_PART(part) \
	dbus_message_iter_get_basic (&status, &ret->part); \
dbus_message_iter_next (&status);

static MPRISPlayerStatus*
demarshal_status (DBusMessage* msg)
{
	DBusMessageIter args, status;
	MPRISPlayerStatus* ret = malloc(sizeof (MPRISPlayerStatus));

	dbus_message_iter_init(msg, &args);
	dbus_message_iter_recurse(&args, &status);

	GET_STATUS_PART (state);
	GET_STATUS_PART (random);
	GET_STATUS_PART (repeat);
	GET_STATUS_PART (loop);

	return ret;
}

#undef GET_STATUS_PART

static void
handle_TrackChange (DBusMessage* msg, MPRISPlayer* player)
{
	MPRISMetadata* metadata = demarshal_metadata(msg);

	if (player->callback_functions->track_change)
		player->callback_functions->track_change(metadata,
				player, NULL);

	mpris_metadata_free(metadata);
}

static void
handle_CapsChange (DBusMessage* msg, MPRISPlayer* player)
{
	DBusError err;
	int caps;

	dbus_error_init (&err);
	dbus_message_get_args (msg, &err,
			DBUS_TYPE_INT32, &caps,
			DBUS_TYPE_INVALID );

	dbus_error_free (&err);

	if (player->callback_functions->caps_change)
		player->callback_functions->caps_change (caps, player, NULL);
}

static void
handle_StatusChange (DBusMessage* msg, MPRISPlayer* player)
{
	MPRISPlayerStatus* status = demarshal_status(msg);

	if (player->callback_functions->status_change)
		player->callback_functions->status_change(status,
				player, NULL);

	free(status);
}

#define HANDLE_SIGNAL(signal) \
	else if (dbus_message_is_signal (msg, \
				"org.freedesktop.MediaPlayer", #signal)) \
handle_##signal (msg, player);

static DBusHandlerResult
handle_signals (DBusConnection* conn, DBusMessage* msg, void* user_data)
{
	MPRISPlayer *player = (MPRISPlayer*) user_data;

	if (0);
		HANDLE_SIGNAL(TrackChange)
		HANDLE_SIGNAL(StatusChange)
		HANDLE_SIGNAL(CapsChange)
	return 0;
}

#undef HANDLE_SIGNAL

MPRISMetadata*
mpris_metadata_get(MPRISPlayer *player, int track)
{
	char *p_name;
	DBusMessage *msg;
	MPRISMetadata *metadata = malloc(sizeof(MPRISMetadata));
	memset(metadata, 0x00, sizeof(MPRISMetadata));

	p_name = player->p_info->suffix;

	msg = mpris_dbus_get_metadata_msg(p_name, track);

	if (!msg)
		return NULL;

	metadata = demarshal_metadata(msg);

	return metadata;

}

MPRISMetadata*
mpris_metadata_get_current_track(MPRISPlayer *player)
{
	int track;
	char *p_name;

	p_name = player->p_info->suffix;

	track = mpris_dbus_get_current_track(p_name);

	return mpris_metadata_get(player, track);
}


void mpris_player_next(MPRISPlayer *player)
{
	mpris_dbus_single_call(player->p_info->suffix, "Next");
}

void mpris_player_prev(MPRISPlayer *player)
{
	mpris_dbus_single_call(player->p_info->suffix, "Prev");
}

void mpris_player_pause(MPRISPlayer *player)
{
	mpris_dbus_single_call(player->p_info->suffix, "Pause");
}

void mpris_player_stop(MPRISPlayer *player)
{
	mpris_dbus_single_call(player->p_info->suffix, "Stop");
}

void mpris_player_play(MPRISPlayer *player)
{
	mpris_dbus_single_call(player->p_info->suffix, "Play");
}

void mpris_tracklist_set_loop(MPRISPlayer *player, int boolean)
{
	mpris_dbus_set_loop(player->p_info->suffix, boolean);
}

void mpris_tracklist_set_random(MPRISPlayer *player, int boolean)
{
	mpris_dbus_set_random(player->p_info->suffix, boolean);
}

void mpris_tracklist_del_track(MPRISPlayer *player, int index)
{
	mpris_dbus_del_track(player->p_info->suffix, index);
}

int mpris_tracklist_add_track(MPRISPlayer *player, const char *uri, int play)
{
	mpris_dbus_add_track(player->p_info->suffix, uri, play);
}

int mpris_tracklist_get_length(MPRISPlayer *player)
{
	return mpris_dbus_get_length(player->p_info->suffix);
}
