#include <stddef.h>
#include <stdlib.h>
#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

MPRISPlayerInfo**
mpris_list (void)
{
  struct list_head *head;
  MPRISPlayerInfo **list;
  MPRISPlayerInfo *item;
  int count = 0;

  head = mpris_dbus_list ();

  list_for_each_entry (item, head, node)
    {
      count++;
    }

  list = malloc ((sizeof (MPRISPlayerInfo*) * count) + 1);
  count = 0;

  list_for_each_entry (item, head, node)
    {
      list[count++] = item;
    }
  list[count] = NULL;
  
  return list;
}

