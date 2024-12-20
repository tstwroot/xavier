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

#define LICENSE "GPL-3.0"

#ifndef XAVIER_MEMORY
#define XAVIER_MEMORY

#include <sys/types.h>

struct mlist;

struct mlist *mlist_new();
char mlist_insert(struct mlist *, void *addr, char);

void mscan(pid_t pid);

XAVIER_MEMORY
#endif
