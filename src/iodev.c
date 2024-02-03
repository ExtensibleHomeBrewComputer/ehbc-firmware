#include <ehbc/iodev.h>
#include <ehbc/hw/device.h>

static void* iodev = NULL;
static const DeviceTrait* iodev_trait = NULL;

int set_io_device(void* dev, const DeviceTrait* dev_trait)
{
    if (dev == NULL || dev_trait == NULL) {
        return -1;
    }

    iodev = dev;
    iodev_trait = dev_trait;
    return 0;
}

int get_io_device(void** dev, const DeviceTrait** dev_trait)
{
    if (iodev == NULL || iodev_trait == NULL) {
        return -1;
    }

    *dev = iodev;
    *dev_trait = iodev_trait;

    return 0;
}
