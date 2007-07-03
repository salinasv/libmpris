/*
 * LibMPRIS (C) 2007 deadchip, mirsal
 */

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "mpris/list.h"

MPRISList*
mpris_list_head (MPRISList * list)
{
  MPRISList * head = list;
  while (head->prev)
  {
    head = head->prev;
  } 
  return head;
}

MPRISList*
mpris_list_tail (MPRISList * list)
{
  MPRISList * tail = list;
  while (tail->next)
  {
    tail = tail->next;
  } 
  return tail;
}

size_t
mpris_list_size (MPRISList * list)
{
  MPRISList * head = mpris_list_head (list);
  size_t size = 0;
  while (head->next)
  {
    head = head->next;
    ++size;
  }
  return size;
}

MPRISList*
mpris_list_new ()
{
  MPRISList * list = malloc (sizeof(MPRISList));
  memset (list, 0x00, sizeof(MPRISList));
  return list;
}

void
mpris_list_free (MPRISList * list, MPRISListDataDestroy destroy)
{
  MPRISList * head1 = mpris_list_head (list);
  MPRISList * head2 = mpris_list_head (list);
  while (head1->next)
  {
    if (destroy)
    {
      destroy (head1->data);
    }
    head1 = head1->next;
  }
  free (head2);
}

void
mpris_list_insert (MPRISList * list, void* data)
{
  MPRISList * item = mpris_list_new (); 
  item->data = data;
  item->next = list->next; 
  item->prev = list;
  list->next = item;
}

void
mpris_list_append (MPRISList * list, void* data)
{
  MPRISList * tail = mpris_list_tail (list); 
  MPRISList * item = mpris_list_new ();
  
  tail->next = item;
  item->prev = tail;
  item->data = data;
}

void
mpris_list_prepend (MPRISList * list, void* data)
{
  MPRISList * head = mpris_list_head (list); 
  MPRISList * item = mpris_list_new ();

  head->prev = item;
  item->next = head;
  item->data = data;
}

void
mpris_list_remove (MPRISList * list, MPRISListDataDestroy destroy)
{
  MPRISList * prev = list->prev; 
  MPRISList * next = list->next; 

  if (destroy)
  {
    destroy (list->data);
  }

  prev->next = next;
  next->prev = prev;
}

void
mpris_list_foreach (MPRISList * list, MPRISListForeachFunc foreach_func, void * data)
{
  MPRISList * head = mpris_list_head (list); 

  while (head->next)
  { 
    foreach_func (head, data); 
    head = head->next;
  }
}

MPRISList*
mpris_list_find (MPRISList * list, MPRISListFindFunc find_func, void * data_key, void * data)
{
  MPRISList * head = mpris_list_head (list); 

  while (head->next)
  { 
    int found = find_func (head, data_key, data); 
    if (found)
    {
      return head;
    }
  }
  return NULL;
}
