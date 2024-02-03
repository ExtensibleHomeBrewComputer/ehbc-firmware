#ifndef _EHBC_IODEV_H__
#define _EHBC_IODEV_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>
#include <ehbc/hw/device.h>

int set_io_device(void* dev, const DeviceTrait* dev_trait);
int get_io_device(void** dev, const DeviceTrait** dev_trait);

#endif  // _EHBC_IODEV_H__
