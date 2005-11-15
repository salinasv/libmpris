#ifndef _MPRIS_DBUS_H_
#define _MPRIS_DBUS_H_

#include <dbus/dbus.h>
#include <mpris/mpris-list.h>

extern DBusConnection  *conn;

DBusConnection*
mpris_dbus_get (void);
 
int
mpris_dbus_init (void);

struct list_head*
mpris_dbus_list (void);

#endif /* _MPRIS_DBUS_H_ */

