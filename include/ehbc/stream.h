#ifndef _EHBC_STREAM_H__
#define _EHBC_STREAM_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>

typedef struct {
    void* mpool;
    size32_t elem_size;
    size32_t elem_count;

    uint32_t iidx;
    uint32_t oidx;
} Stream;

int memberof(Stream, construct)(Stream* self, size32_t elem_size, size32_t elem_count);
int memberof(Stream, destruct)(Stream* self);

int memberof(Stream, push_data)(Stream* self, const void* data_ptr);
int memberof(Stream, pop_data)(Stream* self, void* data_ptr);

int memberof(Stream, is_available)(Stream* self);

const void* memberof(Stream, get_front)(Stream* self);
const void* memberof(Stream, get_back)(Stream* self);

int memberof(Stream, clear)(Stream* self);

#endif  // _EHBC_STREAM_H__
