/*
  Xavier, a linux process memory manipulator 
  Copyright (C) 2024  Pablo Vitorino Panciera (@tstwroot)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <pthread.h>
#include <stdlib.h>

#include "memory.h"

struct mnode
{
  void *addr;
  char data;
  struct mnode *next;
};

struct mlist
{
  struct mnode *start;
  struct mnode *end;
  pthread_rwlock_t lock;
};

struct mlist *
mlist_new()
{
  struct mlist *new_mlist = malloc(sizeof(struct mlist));
  new_mlist->start = NULL;
  new_mlist->end = NULL;
  pthread_rwlock_init(&new_mlist->lock, NULL);
  return new_mlist;
}

static struct mnode *
mlist_new_node(void *addr, char data)
{
  struct mnode *new_node = malloc(sizeof(struct mnode));

  if(new_node == NULL)
    return NULL;

  new_node->addr = addr;
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

char
mlist_insert(struct mlist *m_list, void *addr, char data)
{
  if(m_list == NULL)
    return -1;

  pthread_rwlock_wrlock(&m_list->lock);

  if(m_list->start == NULL)
  {
    m_list->start = mlist_new_node(addr, data);
    if(m_list->start == NULL)
    {
      pthread_rwlock_unlock(&m_list->lock);
      return -1;
    }
    m_list->end = m_list->start;
    pthread_rwlock_unlock(&m_list->lock);
    return 0;
  }
  
  m_list->end->next = mlist_new_node(addr, data);
  if(m_list->end->next == NULL)
  {
    pthread_rwlock_unlock(&m_list->lock);
    return -1;
  }
  m_list->end = m_list->end->next;
  
  pthread_rwlock_wrlock(&m_list->lock);
  return 0;
}
