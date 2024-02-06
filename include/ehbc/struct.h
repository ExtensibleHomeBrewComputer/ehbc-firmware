#ifndef _EHBC_STRUCT_H__
#define _EHBC_STRUCT_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>

#define impl(struct, trait)         S__##struct##_##trait##_impl

#define memberof(struct, member)    S__##struct##_##member

#endif  // _EHBC_STRUCT_H__
