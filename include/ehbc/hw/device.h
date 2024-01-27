#ifndef _EHBC_HW_DEVICE_H__
#define _EHBC_HW_DEVICE_H__

#include <ehbc/types.h>
#include <ehbc/hw/bus.h>
#include <ehbc/class.h>

extern const classdef_t _device_def;

#define Device (&_device_def)

struct _device_vtable {
    ssize32_t (*read)(class_t* self, void* buf, size32_t count, size32_t offset);
    ssize32_t (*write)(class_t* self, const void* buf, size32_t count, size32_t offset);
};

static inline ssize32_t methodof(Device, read)(class_t* self, void* buf, size32_t count, size32_t offset)
{
    return ((const struct _device_vtable*)vtableof(self, Device))->read(self, buf, count, offset);
}
static inline ssize32_t methodof(Device, write)(class_t* self, const void* buf, size32_t count, size32_t offset)
{
    return ((const struct _device_vtable*)vtableof(self, Device))->write(self, buf, count, offset);
}

#endif  // _EHBC_HW_DEVICE_H__
