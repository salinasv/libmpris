#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    char *name;
    char *interface;
    char *path;
};

int
mpris_init (void);

int
mpris_list (struct list_head *players);

#endif /* _MPRIS_H_ */
