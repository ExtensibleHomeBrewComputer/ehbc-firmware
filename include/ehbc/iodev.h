#ifndef _EHBC_IODEV_H__
#define _EHBC_IODEV_H__

#include <ehbc/types.h>
#include <ehbc/memory.h>
#include <ehbc/class.h>

int set_io_device(class_t* device);
class_t* get_io_device(void);

#endif  // _EHBC_IODEV_H__
