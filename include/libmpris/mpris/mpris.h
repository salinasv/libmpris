#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    struct list_head node;

    char *name;
    char *interface;
    char *path;
};

int
mpris_init (void);

MPRISPlayerInfo**
mpris_list_clients (void);

#endif /* _MPRIS_H_ */
