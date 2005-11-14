#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <glib.h>
#include <mpris/dbus.h>

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    gint   id;

    gchar *name;
    gchar *interface;
};

gboolean
mpris_init (void);

GPtrArray*
mpris_list (void);

#endif /* _MPRIS_H_ */
