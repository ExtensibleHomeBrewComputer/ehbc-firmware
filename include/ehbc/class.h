#ifndef _EHBC_CLASS_H__
#define _EHBC_CLASS_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>

typedef int classid_t;

struct class_s;

struct class_parent_entry {
    const struct classdef_s*    def;
    uint32_t                    offset;
    const void*                 vtable;
};

typedef struct classdef_s {
    classid_t*  id;
    size32_t    data_size;
    const char* name;

    void (*constructor)(struct class_s*);
    void (*destructor)(struct class_s*);

    size32_t    parents_count;
    const struct class_parent_entry* parents;
} classdef_t;

typedef struct class_s {
    const classdef_t* def;
    uint8_t     data[];
} class_t;

#define methodof(class, method) __##class##_##method

void* baseof(const class_t* self, const classdef_t* clsdef);
const void* vtableof(const class_t* self, const classdef_t* clsdef);

static classid_t __next_class_id = 0;

class_t* new_class(const classdef_t* clsdef);
static inline void delete_class(class_t* cls)
{
    cls->def->destructor(cls);
    mem_free(cls);
}

static inline int is_equal_class(const classdef_t* a, const classdef_t* b)
{
    return a->id == b->id;
}

static inline int is_base(const classdef_t* base, const classdef_t* derived)
{
    for (int i = 0; i < derived->parents_count; i++) {
        if (derived->parents[i].def->id == base->id) {
            return 1;
        }
    }
    return 0;
}

static inline const char* classnameof(const classdef_t* clsdef)
{
    return clsdef->name;
}

static inline const classdef_t* classof(const class_t* cls)
{
    return cls->def;
}

#endif  // _EHBC_CLASS_H__
