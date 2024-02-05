#ifndef _EHBC_STRUCT_H__
#define _EHBC_STRUCT_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>

#define impl(trait)                 T__##trait##_impl

#define memberof(struct, member)    S__##struct##_##member
#define ftableof(struct)            S__##struct##_ftable

#endif  // _EHBC_STRUCT_H__
