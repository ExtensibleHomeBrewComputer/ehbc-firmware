#include <ehbc/class.h>


class_t* new_class(const classdef_t* clsdef)
{
    size32_t cls_size = sizeof(class_t) + clsdef->data_size;
    for (int i = 0; i < clsdef->parents_count; i++) {
        cls_size += clsdef->parents[i].def->data_size;
    }
    class_t* cls = (class_t*)mem_alloc(cls_size);
    cls->def = clsdef;
    clsdef->constructor(cls);
    return cls;
}

void* baseof(const class_t* self, const classdef_t* clsdef)
{
    if (self->def->id == clsdef->id) {
        return (uint8_t*)self + sizeof(class_t);
    }
    for (int i = 0; self->def->parents_count; i++) {
        if (self->def->parents[i].def->id == clsdef->id) {
            return (uint8_t*)self + sizeof(class_t) + self->def->parents[i].offset;
        }
    }
    return NULL;
}

const void* vtableof(const class_t* self, const classdef_t* clsdef)
{
    for (int i = 0; self->def->parents_count; i++) {
        if (self->def->parents[i].def->id == clsdef->id) {
            return self->def->parents[i].vtable;
        }
    }
    return NULL;
}
