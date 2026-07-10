#include "memory.h"

//static unsigned char malloc_mem[1024 * 768 * 4 * 2];
//static size_t mem_idx = 0;

extern uint8_t end[];
static uint8_t *heap_start;
static size_t mem_idx = 0;

#define HEAP_SIZE (1024 * 768 * 4 * 2)

int get_mem_idx() { return mem_idx; }

void k_heap_init()
{
    heap_start = (uint8_t *) end;
    mem_idx = 0;
}

void k_memcpy(void *dest, void *src, size_t nbytes)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < nbytes; i++)
    {
        d[i] = s[i];
    }
}

void k_memcpy_adv(void *dest, const void *src, size_t nbytes)
{
    uint32_t count = nbytes / 4;
    asm volatile(
        "rep movsl"
        : "+D"(dest), "+S"(src), "+c"(count)
        :
        : "memory"
    );
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
    if (mem_idx + size > HEAP_SIZE)
    {
        return NULL;
    }
    void *mem = heap_start + mem_idx;
    mem_idx += size;
    
    return mem;
}

void k_free()
{
    
}