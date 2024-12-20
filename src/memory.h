#ifndef XAVIER_MEMORY
#define XAVIER_MEMORY

#include <sys/types.h>

struct mlist;

struct mlist *mlist_new();
char mlist_insert(struct mlist *, void *addr, char);

void mscan(pid_t pid);

XAVIER_MEMORY
#endif
