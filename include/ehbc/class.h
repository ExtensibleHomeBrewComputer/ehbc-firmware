#ifndef _EHBC_CLASS_H__
#define _EHBC_CLASS_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>

typedef int classid_t;

struct class_s;

typedef struct {
    struct classdef_s*  def;
    uint32_t            offset;
} class_parent_t;

typedef struct classdef_s {
    classid_t   id;
    size32_t    data_size;

    void (*constructor)(struct class_s*);
    void (*destructor)(struct class_s*);

    size32_t    parents_count;
    const class_parent_t* parents;
} classdef_t;

typedef struct class_s {
    const classdef_t* def;
    uint8_t     data[];
} class_t;

#define methodof(class, method) __##class##_##method

static inline void* baseof(const class_t* self, const classdef_t* clsdef)
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

static classid_t __next_class_id = 0;

static inline class_t* new_class(const classdef_t* clsdef)
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

static inline void delete_class(class_t* cls)
{
    cls->def->destructor(cls);
    mem_free(cls);
}

static inline int is_equal_class(const classdef_t* a, const classdef_t* b)
{
    return a->id == b->id;
}

static inline int is_base_of(const classdef_t* base, const classdef_t* derived)
{
    for (int i = 0; i < base->parents_count; i++) {
        if (base->parents[i].def->id == derived->id) {
            return 1;
        }
    }
    return 0;
}

#endif  // _EHBC_CLASS_H__
