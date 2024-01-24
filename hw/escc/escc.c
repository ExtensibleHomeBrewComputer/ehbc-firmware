#include <ehbc/hw/escc.h>
#include <ehbc/hw/device.h>
#include <ehbc/hw/bus.h>

struct escc_properties {
    uint8_t* cmd_register_a;
    uint8_t* data_register_a;
    uint8_t* cmd_register_b;
    uint8_t* data_register_b;
};

static const uint8_t init_sequence_8n1[] = {
    0x04, 0xC4,     /* no parity, 1 stop bit, clock x64 */
    0x0E, 0x04,     /* auto echo */
    0x03, 0xC1,     /* 8bpc for rx, enable rx */
    0x05, 0x68,     /* 8bpc for tx, enable tx */
};

void debug_print(const char* str);

static void escc_constructor(class_t* self)
{
    Device->constructor(self);
}

static void escc_destructor(class_t* self)
{
    Device->destructor(self);
}

void methodof(DeviceESCC, initialize_channel_a)(class_t* self, uint8_t* cmd_register, uint8_t* data_register)
{
    struct escc_properties* self_p = (struct escc_properties*)baseof(self, DeviceESCC);
    self_p->cmd_register_a = cmd_register;
    self_p->data_register_a = data_register;

    for (int i = 0; i < sizeof(init_sequence_8n1); i++) {
        *self_p->cmd_register_a = init_sequence_8n1[i];
    }
}

void methodof(DeviceESCC, print_channel_a)(class_t* self, const char* str)
{
    struct escc_properties* self_p = (struct escc_properties*)baseof(self, DeviceESCC);
    for (; *str != 0; str++) {
        *self_p->data_register_a = *str;
    }
}

static const class_parent_t escc_parents[] = {
    { Device, sizeof(struct escc_properties) }
};

classdef_t class_device_escc_def = {
    .constructor = escc_constructor,
    .destructor = escc_destructor,
    .parents = escc_parents,
    .parents_count = 1,
};

__attribute__((constructor))
static void __class_def_DeviceESCC(void) {
    class_device_escc_def.id = ++__next_class_id;
    class_device_escc_def.data_size = sizeof(struct escc_properties);
}
