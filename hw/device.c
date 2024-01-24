#include <ehbc/hw/device.h>

struct device_properties {
    uint16_t ready : 1;
};

void debug_print(const char* str);

static void device_constructor(class_t* self)
{
    struct device_properties* self_p = (struct device_properties*)baseof(self, Device);
    self_p->ready = 1;
}

static void device_destructor(class_t* self)
{

}

classdef_t class_device_def = {
    .constructor = device_constructor,
    .destructor = device_destructor,
    .parents = NULL,
    .parents_count = 0,
};

__attribute__((constructor))
static void __class_def_DeviceESCC(void) {
    class_device_def.id = ++__next_class_id;
    class_device_def.data_size = sizeof(struct device_properties);
}

