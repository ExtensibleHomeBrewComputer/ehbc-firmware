#ifndef _EHBC_MEMORY_H__
#define _EHBC_MEMORY_H__

#include <ehbc/types.h>

#define MALLOC_BLOCKSIZE (1 * kiB)

void* mem_alloc(size32_t size);
void mem_free(void* ptr);

#endif  // _EHBC_MEMORY_H__
