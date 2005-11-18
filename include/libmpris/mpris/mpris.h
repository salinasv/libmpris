#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <mpris/mpris-list.h>

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    struct list_head node;

    char *suffix;
    char *name;
    char *interface;
    char *path;
};

typedef struct _MPRISClient MPRISClient;

typedef void (*MPRIS_CbSetStreamPos)   (MPRISClient *client, int32_t stream_pos);
typedef void (*MPRIS_CbTrackChange)    (MPRISClient *client);
typedef void (*MPRIS_CbSetPlaystatus)  (MPRISClient *client, int32_t playstatus);
typedef void (*MPRIS_CbSetVolume)      (MPRISClient *client, int32_t volume);

struct _MPRISClient {
    MPRISPlayerInfo	  *p_info;

    MPRIS_CbSetStreamPos   cb_set_stream_pos;
    MPRIS_CbTrackChange    cb_track_change;
    MPRIS_CbSetPlaystatus  cb_set_playstatus;
    MPRIS_CbSetVolume	   cb_set_volume;

    int			   t_exit;
};

int
mpris_init (void);

MPRISClient*
mpris_client_new (const char *player,
		  MPRIS_CbSetStreamPos  cb_set_stream_pos,
		  MPRIS_CbTrackChange   cb_track_change,
		  MPRIS_CbSetPlaystatus cb_set_playstatus,
		  MPRIS_CbSetVolume	cb_set_volume);

MPRISPlayerInfo**
mpris_list_clients (void);

#endif /* _MPRIS_H_ */
