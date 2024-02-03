#ifndef _EHBC_STRUCT_H__
#define _EHBC_STRUCT_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>

#define impl(trait) __##trait##_impl
#define methodof(struct, method) __##struct##_##method
#define ftableof(struct) __##struct##_ftable

#endif  // _EHBC_STRUCT_H__
