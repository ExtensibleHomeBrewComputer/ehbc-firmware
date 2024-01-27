#ifndef _EHBC_HW_ATA_H__
#define _EHBC_HW_ATA_H__

#include <ehbc/types.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

extern const classdef_t _device_ata_def;

#define DeviceATA (&_device_ata_def)

int methodof(DeviceATA, initialize)(class_t* self, hwreg16_t* reg_base);
void methodof(DeviceATA, get_identifier_struct)(class_t* self, void* struct_buf);
int methodof(DeviceATA, read_sectors_pio)(class_t* self, void* buf, uint32_t lba, uint8_t count);
int methodof(DeviceATA, write_sectors_pio)(class_t* self, const void* buf, uint32_t lba, uint8_t count);

#endif  // _EHBC_HW_ATA_H__
