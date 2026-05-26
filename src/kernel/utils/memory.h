#ifndef KUTILSMEM
#define KUTILSMEM

#include <stddef.h>

int get_mem_idx();
void k_memcpy(unsigned char *dest, unsigned char *src, size_t nbytes);
void *k_malloc(size_t size);

#endif