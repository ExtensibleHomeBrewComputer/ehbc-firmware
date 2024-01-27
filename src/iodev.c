#include <ehbc/iodev.h>
#include <ehbc/hw/device.h>

static class_t* iodev;

int set_io_device(class_t* device)
{
    if (device == NULL || !is_base(Device, device->def)) {
        return -1;
    }

    iodev = device;
    return 0;
}

class_t* get_io_device(void)
{
    return iodev;
}
