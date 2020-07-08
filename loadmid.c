#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "loadmid.h"

// global midi pointer

int loadmid(const char *filename)
{
 
  struct stat stat_buf;
  int fd=0;

  stat(filename, &stat_buf);

  fd=open(filename, O_RDONLY);

  midi_start = mmap(NULL, (size_t)stat_buf.st_size, PROT_READ, MAP_PRIVATE,
                  fd, 0);


  return 0;

}
