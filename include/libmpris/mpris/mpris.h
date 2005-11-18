#ifndef _MPRIS_H_
#define _MPRIS_H_

#include <mpris/mpris-list.h>

typedef enum {
    PLAYSTATUS_STOPPED = 1 << 1,
    PLAYSTATUS_PLAYING = 1 << 2,
    PLAYSTATUS_PAUSED  = 1 << 3,
    PLAYSTATUS_SEEKING = 1 << 4,
    PLAYSTATUS_WAITING = 1 << 5
} MPRISPlaystatus;

typedef struct _MPRISPlayerInfo MPRISPlayerInfo;
struct _MPRISPlayerInfo {
    struct list_head node;

    char *suffix;
    char *name;
    char *interface;
    char *path;
};

typedef struct _MPRISPlayer MPRISPlayer;

typedef void (*MPRIS_CbSetStreamPos)   (MPRISPlayer *player, int32_t stream_pos);
typedef void (*MPRIS_CbTrackChange)    (MPRISPlayer *player);
typedef void (*MPRIS_CbSetPlaystatus)  (MPRISPlayer *player, int32_t playstatus);
typedef void (*MPRIS_CbSetVolume)      (MPRISPlayer *player, int32_t volume);

struct _MPRISPlayer {
    MPRISPlayerInfo	  *p_info;

    MPRIS_CbSetStreamPos   cb_set_stream_pos;
    MPRIS_CbTrackChange    cb_track_change;
    MPRIS_CbSetPlaystatus  cb_set_playstatus;
    MPRIS_CbSetVolume	   cb_set_volume;

    int			   t_exit;
};

int
mpris_client_init (void);

int
mpris_server_init (void);

MPRISPlayer*
mpris_player_new (const char *player,
		  MPRIS_CbSetStreamPos  cb_set_stream_pos,
		  MPRIS_CbTrackChange   cb_track_change,
		  MPRIS_CbSetPlaystatus cb_set_playstatus,
		  MPRIS_CbSetVolume	cb_set_volume);

MPRISPlayerInfo**
mpris_list_players (void);

#endif /* _MPRIS_H_ */
