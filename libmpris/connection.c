#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <mpris/mpris.h>
#include <mpris/dbus.h>
#include <mpris/connection.h>

static char * method_names[] =
{
      "PlayNext",
      "PlayPrev",
      "PlayPause",
      "PlayStop",
      "PlayCurrent",
      "GetCurrentTitle",
      "GetCurrentTrack",
      "VolumeGet",
      "VolumeSet"
};
