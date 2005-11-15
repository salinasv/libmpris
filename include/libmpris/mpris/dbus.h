#ifndef _MPRIS_DBUS_H_
#define _MPRIS_DBUS_H_

#include <glib.h>
#include <glib-object.h>

#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

extern DBusGConnection  *mpris_bus;

DBusGConnection*
mpris_dbus_get (void);
 
gboolean
mpris_dbus_init (void);

GList*
mpris_dbus_list (void);

#endif /* _MPRIS_DBUS_H_ */

