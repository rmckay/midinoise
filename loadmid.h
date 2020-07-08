#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// global midi pointer

void *midi_start;

int loadmid(const char *filename);
