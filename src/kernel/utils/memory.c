#include "memory.h"

static unsigned char malloc_mem[1024 * 1024];
static size_t mem_idx = 0;

int get_mem_idx() { return mem_idx; }

void k_memcpy(unsigned char *dest, unsigned char *src, size_t nbytes)
{
    for (size_t i = 0; i < nbytes; i++)
    {
        *(dest + i) = *(src + i);
    }
}

void *k_malloc(size_t size)
{
    void *mem = NULL;

    if ((sizeof(malloc_mem) - mem_idx) < size)
    {
        return mem;
    }
    
    mem = &malloc_mem[mem_idx];
    mem_idx += size;

    return mem;
}

void k_free()
{
    
}