#ifndef _EHBC_STACK_H__
#define _EHBC_STACK_H__

#include <ehbc/units.h>
#include <ehbc/memory.h>

/* Base address of initial CPU stack */
#define STACK_BOTTOM        HEAP_TOP;

/* Top address of initial CPU stack */
#define STACK_INITIAL_TOP   1 * MiB + 256 * kiB

static inline size32_t get_stack_free_space()
{
    int v;
    return (size32_t)(&v) - (size32_t)STACK_BOTTOM;
}

#endif  // _EHBC_STACK_H__
