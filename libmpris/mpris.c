#include <mpris/mpris.h>
#include <mpris/dbus.h>

gboolean
mpris_init (void)
{
  return mpris_dbus_init ();
}

GPtrArray*
mpris_list (void)
{
  return mpris_dbus_list ();
}
