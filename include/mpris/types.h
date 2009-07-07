/*****************************************************************************
 * types.h : libmpris data types                                        header 
 *****************************************************************************
 * LibMPRIS - Copyright (C) 2007 Milosz Derezynski, Mirsal Ennaime
 *
 * Authors:    Milosz Derezynski
 *             Mirsal Ennaime <mirsal dot ennaime at gmail dot com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifndef _MPRIS_TYPES_H_
#define _MPRIS_TYPES_H_

#include <pthread.h>
#include <mpris/list.h>
#include <string.h>
#include <stdint.h>

typedef struct _MPRISCallbackFuncs MPRISCallbackFuncs;

typedef enum
{
	MPRIS_STATE_PLAYING,
	MPRIS_STATE_PAUSED,
	MPRIS_STATE_STOPPED
} MPRISPlayerState;

typedef struct _MPRISVersion
{
	int major;
	int minor;
} MPRISVersion;

typedef struct _MPRISPlayerInfo
{
	MPRISList * node;

	char *suffix;
	char *name;
	MPRISVersion mpris_version;

} MPRISPlayerInfo;

typedef struct _MPRISPlayer
{
	MPRISPlayerInfo       * p_info;
	pthread_mutex_t       * lock;
	pthread_t             * listen_thread;
	MPRISCallbackFuncs    * callback_functions;
	int                     thread_exit;
	void                  * user_data;

} MPRISPlayer;

typedef struct _MPRISMetadata
{
	char* location;
	char* title;
	char* artist;
	char* album;
	char* tracknumber;
	uint32_t time;
	uint32_t mtime;
	char* genere;
	char* comment;
	uint32_t rating;
	uint32_t year;
	uint32_t date;
	char* aturl;
} MPRISMetadata;

typedef struct _MPRISPlayerStatus
{
	MPRISPlayerState state;
	int random;
	int repeat;
	int loop;
} MPRISPlayerStatus;

typedef void (* MPRISCallbackTrackChange) (MPRISMetadata*, MPRISPlayer*, void*);
typedef void (* MPRISCallbackStatusChange) (MPRISPlayerStatus*, MPRISPlayer*, void*);
typedef void (* MPRISCallbackCapsChange) (int, MPRISPlayer*, void*);

struct _MPRISCallbackFuncs
{
	MPRISCallbackTrackChange  track_change;
	MPRISCallbackStatusChange status_change;
	MPRISCallbackCapsChange   caps_change;
};

#endif /* _MPRIS_TYPES_H_ */
