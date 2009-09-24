/*****************************************************************************
 * dbus.c : Low level dbus stuff 
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

#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/list.h>

#define MPRIS_ROOT_PATH         "/"
#define MPRIS_TRACKLIST_PATH 	"/TrackList"
#define MPRIS_PLAYER_PATH       "/Player"
#define MPRIS_BUS_NAME_PREFIX   "org.mpris."
#define MPRIS_FDO_IFACE_NAME    "org.freedesktop.MediaPlayer"

DBusConnection * conn = NULL;

static char**
demarshal_strv (DBusMessageIter	* iter)
{
	DBusMessageIter iter2;
	char **ret;
	int current_type;
	int len;
	int i;

	current_type = dbus_message_iter_get_arg_type (iter);

	if (current_type != DBUS_TYPE_ARRAY)
		return NULL;

	dbus_message_iter_recurse (iter, &iter2);
	current_type = dbus_message_iter_get_arg_type (&iter2);

	if (current_type != DBUS_TYPE_INVALID && current_type != DBUS_TYPE_STRING)
		return NULL;

	len = dbus_message_iter_get_array_len (&iter2);
	ret = malloc (sizeof (char *) * (len + 1));

	i = 0;
	while ((current_type = dbus_message_iter_get_arg_type (&iter2)) != DBUS_TYPE_INVALID) {
		const char * aux;
		dbus_message_iter_get_basic (&iter2, &aux);
		ret[i] = strdup (aux);
		dbus_message_iter_next (&iter2);
		i++;
	}
	ret[i] = NULL; /* terminate StrV with NULL */
	return ret;
}

#define GET_VERSION_PART(part) \
	dbus_message_iter_get_basic (&version, &part); \
ret.part = (int) part; \
dbus_message_iter_next (&version);

static MPRISVersion
demarshal_version (DBusMessage* msg)
{
	DBusMessageIter args, version;
	dbus_uint16_t major, minor;
	MPRISVersion ret;

	dbus_message_iter_init (msg, &args);
	dbus_message_iter_recurse (&args, &version);

	GET_VERSION_PART (major);
	GET_VERSION_PART (minor);

	return ret;
}

void mpris_dbus_connection_set(DBusConnection *connection)
{
	conn = connection;
}

int
mpris_dbus_init (void)
{
	DBusError err;
	dbus_error_init (&err);

	if (!conn)
		conn = dbus_bus_get (DBUS_BUS_SESSION, &err);

	if (!conn) {
		printf("dbus: %s\n", err.message);
		return 0;
	}

	return 1;
}

MPRISPlayerInfo*
mpris_dbus_get_player_info (const char * player)
{
	MPRISPlayerInfo   * p_info = malloc (sizeof(MPRISPlayerInfo));
	memset (p_info, 0x00, sizeof(MPRISPlayerInfo));
	DBusMessage       * in = NULL, * out = NULL; /* in as in "into DBus"
													and out as in "from DBus" */

	char* name = (char*) malloc (strlen ("org.mpris.") + strlen (player) + 1);
	sprintf (name, "org.mpris.%s", player);

	DBusError err;
	dbus_error_init (&err);

	if (!dbus_bus_name_has_owner (conn, name, &err)) {
		free (name);
		return NULL;
	}

	in = dbus_message_new_method_call (name, MPRIS_ROOT_PATH,
			MPRIS_FDO_IFACE_NAME, "Identity");
	out = dbus_connection_send_with_reply_and_block (conn, in,
			500 /* let's wait half a second max */, &err);
	dbus_message_unref (in);

	if ((out == NULL) || dbus_error_is_set (&err)) {
		fprintf (stderr, "%s:%d: Message call failed!\n", __FILE__, __LINE__);
		free (name);
		return NULL;
	}

	const char * p_name;

	if (!dbus_message_get_args (out, &err, DBUS_TYPE_STRING,
				&p_name, DBUS_TYPE_INVALID)) {
		fprintf (stderr, "%s:%d: Couldn't get args from message!\n",
				__FILE__, __LINE__);
		free (name);
		dbus_message_unref (out);
		return NULL;
	}

	dbus_message_unref (out);

	p_info->name = strdup (p_name);
	p_info->suffix = strdup (player);

	in = dbus_message_new_method_call (name, MPRIS_ROOT_PATH,
			MPRIS_FDO_IFACE_NAME, "MprisVersion");
	out = dbus_connection_send_with_reply_and_block (conn, in,
			500 /* let's wait half a second max */, &err);
	dbus_message_unref (in);
	free (name);

	if ((out == NULL) || dbus_error_is_set (&err)) {
		fprintf (stderr, "%s:%d: Message call failed!\n", __FILE__, __LINE__);
		return NULL;
	}


	DBusMessageIter iter;

	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return NULL;
	}

	if (DBUS_TYPE_STRUCT != dbus_message_iter_get_arg_type (&iter)) {
		fprintf (stderr, "%s:%d: Wrong reply type!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return NULL;
	}

	p_info->mpris_version = demarshal_version (out);
	dbus_message_unref (out);

	return p_info;
}

MPRISList*
mpris_dbus_list_players (void)
{
	MPRISList* players = mpris_list_new ();

	DBusMessage * in = NULL, * out = NULL; /* in as in "into DBus" and out as in "from DBus" */

	DBusError err;
	dbus_error_init (&err);

	in = dbus_message_new_method_call ("org.freedesktop.DBus",
			"/org/freedesktop/DBus", "org.freedesktop.DBus", "ListNames");

	out = dbus_connection_send_with_reply_and_block (conn, in,
			500 /* let's wait half a second max */, &err);
	dbus_message_unref (in);

	if ((out == NULL) || dbus_error_is_set (&err)) {
		fprintf (stderr, "%s:%d: Message call failed!\n", __FILE__, __LINE__);
		return NULL;
	}


	DBusMessageIter iter;
	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
		return NULL;
	}

	char ** names = NULL;
	if (DBUS_TYPE_ARRAY == dbus_message_iter_get_arg_type (&iter)) {
		names = demarshal_strv (&iter);
	} else {
		dbus_message_unref (out);
		return NULL;
	}

	int n = 0;
	while (names[n]) {
		if (!strncasecmp (MPRIS_BUS_NAME_PREFIX, names[n],
					strlen (MPRIS_BUS_NAME_PREFIX)))
		{
			MPRISPlayerInfo * p_info = NULL;
			char * rstring = rindex (names[n], '.');
			rstring++;
			p_info = mpris_dbus_get_player_info (rstring);
			mpris_list_append (players, p_info);
			p_info->node = mpris_list_tail (players);
		}
		++n;
	}

	dbus_message_unref (out);
	return players;
}

static char*
create_destination_name(const char *player)
{
	char *name = (char*) malloc(strlen("org.mpris") + strlen(player) + 1);
	sprintf(name, "org.mpris.%s", player);

	return name;
}

DBusMessage*
mpris_dbus_get_metadata_msg(const char *player, int track)
{
	DBusMessage *in = NULL; /* in as "into DBus" */
	DBusMessage *out = NULL; /* out as "from DBus" */
	DBusError err;

	char *name = create_destination_name(player);

	dbus_error_init(&err);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "GetMetadata");

	if(!dbus_message_append_args(in, DBUS_TYPE_INT32, &track, DBUS_TYPE_INVALID))
		printf("Error appending args to message\n");

	out = dbus_connection_send_with_reply_and_block(conn, in,
			500, &err);

	dbus_message_unref(in);
	free(name);

	if ((out == NULL) || dbus_error_is_set(&err)) {
		fprintf (stderr, "%s:%d: Message call failed\n", __FILE__, __LINE__);
		return NULL;
	}

	DBusMessageIter iter;
	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return NULL;
	}

	return out;
}

int
mpris_dbus_get_current_track(const char *player)
{
	int ret;
	MPRISMetadata *metadata = malloc(sizeof(MPRISMetadata));
	memset(metadata, 0x00, sizeof(MPRISMetadata));
	DBusMessage *in = NULL; /* in as "into DBus" */
	DBusMessage *out = NULL; /* out as "from DBus" */
	DBusError err;

	char *name = create_destination_name(player);

	dbus_error_init(&err);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "GetCurrentTrack");

	out = dbus_connection_send_with_reply_and_block(conn, in,
			500, &err);

	dbus_message_unref(in);
	free(name);

	if ((out == NULL) || dbus_error_is_set(&err)) {
		fprintf (stderr, "%s:%d: Message call failed\n", __FILE__, __LINE__);
		return -1;
	}

	DBusMessageIter iter;
	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return -1;
	}

	if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type (&iter)) {
		fprintf (stderr, "%s:%d: Wrong reply type!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return -1;
	}

	dbus_message_iter_get_basic(&iter, &ret);

	return ret;
}

void
mpris_dbus_single_call(const char *player, const char *method)
{
	DBusMessage *in = NULL;

	char *name = create_destination_name(player);

	in = dbus_message_new_method_call(name, MPRIS_PLAYER_PATH,
			MPRIS_FDO_IFACE_NAME, method);

	if (!dbus_connection_send(conn, in, NULL))
		printf("Error sending the %s call.\n", method);
}


void
mpris_dbus_set_loop(const char *player, int boolean)
{
	DBusMessage *in = NULL; /* in as "into DBus" */

	char *name = create_destination_name(player);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "SetLoop");

	if(!dbus_message_append_args(in, DBUS_TYPE_BOOLEAN, &boolean, DBUS_TYPE_INVALID))
    {
		fputs("Error appending args to message\n", stderr);
        goto clean;
    }

	if (!dbus_connection_send(conn, in, NULL))
		fputs("Error sending the SetLoop call.\n", stderr);

clean:
	dbus_message_unref(in);
	free(name);
}

void
mpris_dbus_set_random(const char *player, int boolean)
{
	DBusMessage *in = NULL; /* in as "into DBus" */

	char *name = create_destination_name(player);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "SetRandom");

	if(!dbus_message_append_args(in, DBUS_TYPE_BOOLEAN, &boolean, DBUS_TYPE_INVALID))
    {
		fputs("Error appending args to message\n", stderr);
        goto clean;
    }

	if (!dbus_connection_send(conn, in, NULL))
		fputs("Error sending the SetRandom call.\n", stderr);

clean:
	dbus_message_unref(in);
	free(name);
}

void
mpris_dbus_del_track(const char *player, int index)
{
	DBusMessage *in = NULL; /* in as "into DBus" */

	char *name = create_destination_name(player);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "DelTrack");

	if(!dbus_message_append_args(in, DBUS_TYPE_INT32, &index, DBUS_TYPE_INVALID))
    {
		fputs("Error appending args to message\n", stderr);
        goto clean;
    }

	if (!dbus_connection_send(conn, in, NULL))
		fputs("Error sending the SetRandom call.\n", stderr);

clean:
	dbus_message_unref(in);
	free(name);
}

int
mpris_dbus_add_track(const char *player, const char *uri, int play)
{
	DBusMessage *in = NULL; /* in as "into DBus" */
	DBusMessage *out = NULL; /* out as "from DBus" */
	DBusError err;
    int ret;

	char *name = create_destination_name(player);

	dbus_error_init(&err);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "AddTrack");

	if(!dbus_message_append_args(in,
            DBUS_TYPE_STRING, &uri,
            DBUS_TYPE_BOOLEAN, &play,
            DBUS_TYPE_INVALID))
    {
		fputs("Error appending args to message\n", stderr);
        goto error;
    }

	out = dbus_connection_send_with_reply_and_block(conn, in,
			500, &err);

	dbus_message_unref(in);
	free(name);

	if ((out == NULL) || dbus_error_is_set(&err)) {
		fprintf (stderr, "%s:%d: Message call failed\n", __FILE__, __LINE__);
		goto error;
	}

	DBusMessageIter iter;
	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
        goto error;
	}

	if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type (&iter)) {
		fprintf (stderr, "%s:%d: Wrong reply type!\n",
				__FILE__, __LINE__);
        goto error;
	}

	dbus_message_iter_get_basic(&iter, &ret);

	return ret;

error:
	dbus_message_unref(in);
	dbus_message_unref(out);
	free(name);

    return -1;
}

int
mpris_dbus_get_length(const char *player)
{
	int ret;
	MPRISMetadata *metadata = malloc(sizeof(MPRISMetadata));
	memset(metadata, 0x00, sizeof(MPRISMetadata));
	DBusMessage *in = NULL; /* in as "into DBus" */
	DBusMessage *out = NULL; /* out as "from DBus" */
	DBusError err;

	char *name = create_destination_name(player);

	dbus_error_init(&err);

	in = dbus_message_new_method_call(name, MPRIS_TRACKLIST_PATH,
			MPRIS_FDO_IFACE_NAME, "GetLength");

	out = dbus_connection_send_with_reply_and_block(conn, in,
			500, &err);

	dbus_message_unref(in);
	free(name);

	if ((out == NULL) || dbus_error_is_set(&err)) {
		fprintf (stderr, "%s:%d: Message call failed\n", __FILE__, __LINE__);
		return -1;
	}

	DBusMessageIter iter;
	if (!dbus_message_iter_init (out, &iter)) {
		fprintf (stderr, "%s:%d: Couldn't init message iter!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return -1;
	}

	if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type (&iter)) {
		fprintf (stderr, "%s:%d: Wrong reply type!\n",
				__FILE__, __LINE__);
		dbus_message_unref (out);
		return -1;
	}

	dbus_message_iter_get_basic(&iter, &ret);

	return ret;
}

