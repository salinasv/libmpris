#ifndef _MPRIS_DBUS_H_
#define _MPRIS_DBUS_H_

#include <dbus/dbus.h>
#include <mpris/mpris-list.h>

extern DBusConnection  *conn;

int
mpris_dbus_init (void);

struct list_head*
mpris_dbus_list_clients (void);

/* Auxilliary DBus stuff */
int
dbus_message_call_simple (DBusConnection *conn,
			  DBusMessage **msg,
			  const char *target,
			  const char *object,
			  const char *iface,
			  const char *method);
#endif /* _MPRIS_DBUS_H_ */

