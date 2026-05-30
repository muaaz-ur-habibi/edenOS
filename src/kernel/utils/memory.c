#include "memory.h"

static unsigned char malloc_mem[1024 * 768 * 4];
static size_t mem_idx = 0;

int get_mem_idx() { return mem_idx; }

void k_memcpy(void *dest, void *src, size_t nbytes)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < nbytes; i++)
    {
        d[i] = s[i];
    }
}

void *k_memset(void *mem, size_t len, char val)
{
    uint8_t *dst = mem;

    while (len > 0)
    {
        *dst = val;
        dst++;
        len--;
    }
    
    return mem;
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