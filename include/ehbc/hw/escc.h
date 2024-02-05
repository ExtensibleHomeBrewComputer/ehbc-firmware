#ifndef _EHBC_HW_ESCC_H__
#define _EHBC_HW_ESCC_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/hw/device.h>

typedef struct {
    hwreg8_t* cmd_register;
    hwreg8_t* data_register;
} ESCC;

int memberof(ESCC, construct)(void* self, hwreg8_t* cmd_register, hwreg8_t* data_register);
ssize32_t memberof(ESCC, read)(void* self, void* buf, size32_t count, size32_t offset);
ssize32_t memberof(ESCC, write)(void* self, const void* buf, size32_t count, size32_t offset);

static const struct {
    int (*construct)(void* self, hwreg8_t* cmd_register, hwreg8_t* data_register);
    DeviceTrait impl(DeviceTrait);
} ftableof(ESCC) = {
    .construct = memberof(ESCC, construct),
    .impl(DeviceTrait) = {
        .read = memberof(ESCC, read),
        .write = memberof(ESCC, write),
    }
};

#endif  // _EHBC_HW_ESCC_H__
