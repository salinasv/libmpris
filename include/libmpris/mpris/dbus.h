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

