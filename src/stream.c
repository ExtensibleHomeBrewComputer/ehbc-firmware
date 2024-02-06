#include <ehbc/stream.h>

#include <ehbc/memory.h>
#include <ehbc/string.h>

int memberof(Stream, construct)(Stream* self, size32_t elem_size, size32_t elem_count)
{
    self->elem_size = elem_size;
    self->elem_count = elem_count;
    self->mpool = mem_alloc(elem_size * elem_count);

    self->iidx = 0;
    self->oidx = 0;

    return 0;
}

int memberof(Stream, destruct)(Stream* self)
{
    mem_free(self->mpool);
    return 0;
}

int memberof(Stream, push_data)(Stream* self, const void* data_ptr)
{
    memcpy((uint8_t*)self->mpool + self->elem_size * self->iidx++, data_ptr, self->elem_size);
    if (self->iidx >= self->elem_count) {
        self->iidx = 0;
    }

    if (self->iidx == self->oidx) {
        self->oidx++;
        if (self->oidx >= self->elem_count) {
            self->oidx = 0;
        }
    }
    return 0;
}

int memberof(Stream, pop_data)(Stream* self, void* data_ptr)
{
    if (!memberof(Stream, is_available)(self)) {
        return -1;
    }
    memcpy(data_ptr, (uint8_t*)self->mpool + self->elem_size * self->oidx++, self->elem_size);
    if (self->oidx >= self->elem_count) {
        self->oidx = 0;
    }
    return 0;
}

int memberof(Stream, is_available)(Stream* self)
{
    return self->oidx != self->iidx;
}

int memberof(Stream, clear)(Stream* self)
{
    self->iidx = 0;
    self->oidx = 0;
    return 0;
}
