#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/mpris-list.h>

int
mpris_list (struct list_head *players)
{
  return mpris_dbus_list (players);
}
