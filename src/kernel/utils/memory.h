#ifndef KUTILSMEM
#define KUTILSMEM

#include <stddef.h>
#include <stdint.h>

int get_mem_idx();
void k_memcpy(void *dest, void *src, size_t nbytes);
void *k_memset(void *mem, size_t len, char val);
void *k_malloc(size_t size);

#endif