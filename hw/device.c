#include <ehbc/hw/device.h>

struct device_struct {
    uint16_t ready : 1;
};

static void device_constructor(class_t* self)
{
    struct device_struct* self_p = (struct device_struct*)baseof(self, Device);
    self_p->ready = 1;
}

static void device_destructor(class_t* self)
{

}

static classid_t _device_def_id;

const classdef_t _device_def = {
    .id = &_device_def_id,
    .name = "Device",
    .constructor = device_constructor,
    .destructor = device_destructor,
    .parents = NULL,
    .parents_count = 0,
    .data_size = sizeof(struct device_struct),
};


__attribute__((constructor))
static void __class_def_Device(void) {
    _device_def_id = ++__next_class_id;
}
