#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <glib.h>
#include <mpris/dbus.h>

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    gchar *name;
    gchar *interface;
    gchar *path;
};

gboolean
mpris_init (void);

GList*
mpris_list (void);

#endif /* _MPRIS_H_ */
