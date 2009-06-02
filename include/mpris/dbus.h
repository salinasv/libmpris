/*****************************************************************************
 * dbus.h : Low level dbus stuff                                       header 
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
#ifndef _MPRIS_DBUS_H_
#define _MPRIS_DBUS_H_

#include <stdlib.h>
#include <stdio.h>

#include <dbus/dbus.h>
#include <mpris/types.h>

extern DBusConnection  *conn;

int
mpris_dbus_init (void);

MPRISList*
mpris_dbus_list_players (void);

MPRISPlayerInfo*
mpris_dbus_get_player_info (const char *player);

#endif /* _MPRIS_DBUS_H_ */
