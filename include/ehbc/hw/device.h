#ifndef _EHBC_HW_DEVICE_H__
#define _EHBC_HW_DEVICE_H__

#include <ehbc/types.h>
#include <ehbc/hw/bus.h>
#include <ehbc/struct.h>

typedef struct {
    ssize32_t (*read)(void* self, void* buf, size32_t count, size32_t offset);
    ssize32_t (*write)(void* self, const void* buf, size32_t count, size32_t offset);
} DeviceTrait;

#endif  // _EHBC_HW_DEVICE_H__
