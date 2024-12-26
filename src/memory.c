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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "memory.h"

#define PROC_MAPS_BASE_LENGTH 11 // "/proc//maps"
#define MAX_LINE_SIZE 1024

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

int get_number_length(int number)
{
  int counter;
  for(counter = 0; (number /= 10) != 0; counter++);
  return counter + 1;
}

int get_proc_maps_size(pid_t pid)
{
  return PROC_MAPS_BASE_LENGTH + get_number_length(pid);
}

struct memory_range*
search_memory_range(pid_t pid, char *memory_area)
{
  char *proc_maps_filepath = malloc(sizeof(char) * get_number_length(pid));
  sprintf(proc_maps_filepath, "/proc/%d/maps", pid);
  FILE *proc_maps_fp = fopen(proc_maps_filepath, "r");

  if(proc_maps_fp == NULL)
    return NULL;
  
  struct memory_range *memory_range = malloc(sizeof(struct memory_range));
  
  if(memory_range == NULL)
    return NULL;
  
  char line[1024];

  memset(line, 0, 1024);
  
  // TODO: Implement a read line

  fclose(proc_maps_fp);
}
