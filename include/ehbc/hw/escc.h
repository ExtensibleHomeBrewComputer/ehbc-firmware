#ifndef _EHBC_HW_ESCC_H__
#define _EHBC_HW_ESCC_H__

#include <ehbc/types.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

extern classdef_t class_device_escc_def;

#define DeviceESCC (&class_device_escc_def)

void methodof(DeviceESCC, initialize_channel_a)(class_t* self, uint8_t* cmd_register, uint8_t* data_register);
void methodof(DeviceESCC, print_channel_a)(class_t* self, const char* str);

#endif  // _EHBC_HW_ESCC_H__
