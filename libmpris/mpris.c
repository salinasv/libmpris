#include <mpris/mpris.h>
#include <mpris/dbus.h>

GList*
mpris_list (void)
{
  return mpris_dbus_list ();
}
