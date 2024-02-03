#ifndef _EHBC_MEMORY_H__
#define _EHBC_MEMORY_H__

#include <ehbc/types.h>

#define ALLOC_MAXCOUNT 1024

extern int __heap_bottom;
extern void* __heap_top_ptr;

#define HEAP_BOTTOM (&__heap_bottom)
#define HEAP_TOP    ((const int*)__heap_top_ptr)

void* mem_alloc(size32_t size);
void mem_free(void* ptr);

static inline ssize32_t get_used_heap_space(void)
{
    return (ssize32_t)HEAP_TOP - (ssize32_t)HEAP_BOTTOM;
}

#endif  // _EHBC_MEMORY_H__
