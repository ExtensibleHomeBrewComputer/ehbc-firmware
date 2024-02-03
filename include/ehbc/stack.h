#ifndef _EHBC_STACK_H__
#define _EHBC_STACK_H__

#include <ehbc/units.h>
#include <ehbc/memory.h>

/* Base address of initial CPU stack */
#define STACK_BOTTOM        HEAP_TOP;

extern const int __stack_top;

/* Top address of initial CPU stack */
#define STACK_TOP           (&__stack_top)

static inline ssize32_t get_free_stack_space(void)
{
    int v;
    return (ssize32_t)(&v) - (ssize32_t)STACK_BOTTOM;
}

static inline ssize32_t get_used_stack_space(void)
{
    int v;
    return (ssize32_t)STACK_TOP - (ssize32_t)(&v);
}

#endif  // _EHBC_STACK_H__
