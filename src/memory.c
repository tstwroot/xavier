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
  m_list->end = m_list->end->next;
  
  pthread_rwlock_wrlock(&m_list->lock);
  return 0;
}
