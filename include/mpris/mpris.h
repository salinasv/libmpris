/*****************************************************************************
 * mpris.h : libmpris client side dbus wrapper for media players        header
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
#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <pthread.h>
#include <mpris/types.h>

#define MPRIS_PLAYER_IDENTITY(player) \
        player->p_info ? (player->p_info->name) : "NOT CONNECTED"
#define MPRIS_PLAYER_LOCK(player) pthread_mutex_lock (player->lock)
#define MPRIS_PLAYER_UNLOCK(player) pthread_mutex_unlock (player->lock)

int
mpris_client_init   (void);

int
mpris_server_init   (void);

void
mpris_metadata_free (MPRISMetadata*);

MPRISPlayer*
mpris_player_new    (const char *player);

void
mpris_player_free   (MPRISPlayer* player);

MPRISPlayerInfo**
mpris_list_players  (void);

int
mpris_player_start_listen (MPRISPlayer *player);

void
mpris_player_stop_listen (MPRISPlayer *player);

#endif /* _MPRIS_H_ */
