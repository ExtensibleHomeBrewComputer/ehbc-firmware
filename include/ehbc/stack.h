#ifndef _EHBC_STACK_H__
#define _EHBC_STACK_H__

#include <ehbc/units.h>
#include <ehbc/memory.h>

/* Base address of initial CPU stack */
#define STACK_BOTTOM        HEAP_TOP;

/* Top address of initial CPU stack */
#define STACK_INITIAL_TOP   8 * MiB

static inline ssize32_t get_stack_free_space()
{
    int v;
    return (ssize32_t)(&v) - (ssize32_t)STACK_BOTTOM;
}

#endif  // _EHBC_STACK_H__
