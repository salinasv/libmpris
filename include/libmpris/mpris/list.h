/*****************************************************************************
 * 
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
/*
 * LibMPRIS (C) 2007 deadchip, mirsal
 */

#ifndef _LIBMPRIS_LIST_H_
#define _LIBMPRIS_LIST_H_

#include <string.h>
#include <sys/types.h>

typedef struct _MPRISList MPRISList;

struct _MPRISList
{
  void* data;
  MPRISList * next;
  MPRISList * prev;
};

typedef void ( * MPRISListDataDestroy ) (void*);
typedef void ( * MPRISListForeachFunc ) (MPRISList*, void* /* data */, int /* iteration */);
typedef int  ( * MPRISListFindFunc )    (MPRISList*, void* /* key */, void* /* data */);

/* Returns the head of the list */
MPRISList*
mpris_list_head (MPRISList * list);

/* Returns the tail of the list */
MPRISList*
mpris_list_tail (MPRISList * list);

/* Gives the count of N items in the list */
size_t
mpris_list_size (MPRISList * list);

/* Return whether the list is currently an empty list */
int
mpris_list_empty (MPRISList * list);

/* Creates a new list instance (either a head item, or a link item, it's the same anyway) */
MPRISList*
mpris_list_new ();

/* Frees the list. 'destroy' is optional, can be NULL, in which case items will not be destroyed */
void
mpris_list_free (MPRISList * list, MPRISListDataDestroy destroy);

/* Inserts a new link with data /after/ the link item 'list' */
void
mpris_list_insert (MPRISList * list, void* data);

/* Appends an item at the end of the list */
void
mpris_list_append (MPRISList * list, void* data);

/* Prepends an item to the beginning of the list */
void
mpris_list_prepend (MPRISList * list, void* data);

/* Removes the item pointed to by 'list' */
void
mpris_list_remove (MPRISList * list, MPRISListDataDestroy destroy);

/* Calls ForeachFunc on each element of the list, returns the number of iterations */
int
mpris_list_foreach (MPRISList * list, MPRISListForeachFunc foreach_func, void * data);

/* Calls FindFunc which can then inspect the item and return a value != 0 whether to stop iterating */
MPRISList*
mpris_list_find (MPRISList * list, MPRISListFindFunc find_func, void * data_key, void * data);

#endif /* !_LIBMPRIS_LIST_H_ */
