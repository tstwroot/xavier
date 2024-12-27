#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memory.h"
#include "banner.h"

int main(int argc, char **argv)
{
  if(argc != 2)
    return -1;

  puts(XAVIER_BANNER);
  pid_t pid = atoi(argv[1]);
  mscan(pid);
  return 0;
}
