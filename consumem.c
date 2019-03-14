#include <stdio.h>
#include <stdlib.h>


#define BYTES_PER_MEGABYTE 1048576


/*
 * Structs
 */
typedef struct Segment {
  char *segment;
  long acc_allocated;
  struct Segment *next;
} Segment;


/*
 * Functions
 */
Segment *allocate_aux(long bytes_rem, long alloc_size, Segment *s) {
  Segment *ms = malloc(sizeof(Segment));

  char *mem;
  long size = alloc_size;
  while (size >= 1) {

    mem = (char *) calloc(size, sizeof(char));

    if (mem == NULL) {
      size /= 2;
    } else {
      ms->segment = mem;
      ms->acc_allocated = s->acc_allocated + size;
      ms->next = s;

      if (bytes_rem-size > 0) {
        return allocate_aux(bytes_rem-size, size, ms);
      } else {
        return ms;
      }
    }
  }

  return s;
}

Segment *allocate(long bytes) {
  Segment term = {NULL, 0, NULL};
  Segment *mem = allocate_aux(bytes, bytes, &term);

  if (mem->acc_allocated == 0) {
    return NULL;
  }
  return mem;
}


/*** Main ***/
int main(int argc, char **argv)
{
  long megabytes;
  long bytes;
  Segment *mem = malloc(sizeof(Segment));

  char *p;
  if (argc > 1) {
    megabytes = strtol(argv[1], &p, 10);
    bytes = BYTES_PER_MEGABYTE * megabytes;

    printf("Allocating %ldMB (%ld bytes) of memory...\n", megabytes, bytes);
    mem = allocate(bytes);

    if (mem == NULL) {
      printf("Failed to allocate memory!\n");
    } else if (mem->acc_allocated < bytes) {
      printf("Allocated %ldMB (%ld bytes)!\n",
             mem->acc_allocated/BYTES_PER_MEGABYTE,
             mem->acc_allocated);
    } else {
      printf("Allocated memory successfully!\n");
    }

    printf("-- Press [ENTER] to terminate --\n");
    getchar();
  } else {
    printf("usage: %s <MEGABYTES>\n", argv[0]);
  }

  return(0);
}
