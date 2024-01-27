#ifndef _EHBC_HW_ESCC_H__
#define _EHBC_HW_ESCC_H__

#include <ehbc/types.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

extern const classdef_t _device_escc_def;

#define DeviceESCC (&_device_escc_def)

void methodof(DeviceESCC, initialize)(class_t* self, hwreg8_t* cmd_register, hwreg8_t* data_register);
ssize32_t methodof(DeviceESCC, read)(class_t* self, void* buf, size32_t count, size32_t offset);
ssize32_t methodof(DeviceESCC, write)(class_t* self, const void* buf, size32_t count, size32_t offset);

#endif  // _EHBC_HW_ESCC_H__
