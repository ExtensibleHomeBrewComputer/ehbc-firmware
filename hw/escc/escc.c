#include <ehbc/hw/escc.h>
#include <ehbc/hw/device.h>
#include <ehbc/hw/bus.h>

struct escc_struct {
    hwreg8_t* cmd_register;
    hwreg8_t* data_register;
};

static const uint8_t init_sequence_8n1[] = {
    0x04, 0xC4,     /* no parity, 1 stop bit, clock x64 */
    0x0E, 0x04,     /* auto echo */
    0x03, 0xC1,     /* 8bpc for rx, enable rx */
    0x05, 0x68,     /* 8bpc for tx, enable tx */
};

static void escc_constructor(class_t* self)
{
    Device->constructor(self);
}

static void escc_destructor(class_t* self)
{
    Device->destructor(self);
}

static const struct _device_vtable vtbl = {
    .read = NULL,
    .write = methodof(DeviceESCC, write),
};

static const struct class_parent_entry escc_parents[] = {
    { Device, sizeof(struct escc_struct), &vtbl }
};

static classid_t _device_escc_def_id;

const classdef_t _device_escc_def = {
    .id = &_device_escc_def_id,
    .name = "DeviceESCC",
    .constructor = escc_constructor,
    .destructor = escc_destructor,
    .parents = escc_parents,
    .parents_count = 1,
    .data_size = sizeof(struct escc_struct),
};

__attribute__((constructor))
static void __class_def_DeviceESCC(void)
{
    _device_escc_def_id = ++__next_class_id;
}

void methodof(DeviceESCC, initialize)(class_t* self, hwreg8_t* cmd_register, hwreg8_t* data_register)
{
    struct escc_struct* self_p = (struct escc_struct*)baseof(self, DeviceESCC);
    self_p->cmd_register = cmd_register;
    self_p->data_register = data_register;

    for (int i = 0; i < sizeof(init_sequence_8n1); i++) {
        *self_p->cmd_register = init_sequence_8n1[i];
    }
}

ssize32_t methodof(DeviceESCC, read)(class_t* self, void* buf, size32_t count, size32_t offset)
{
    if ((ssize32_t)count < 0 || offset != 0) {
        return -1;
    }
    struct escc_struct* self_p = (struct escc_struct*)baseof(self, DeviceESCC);
    ssize32_t bytes_read = 0;
    uint8_t* tmp = buf;
    while ((bytes_read++) < count && (*self_p->cmd_register & 1)) {
        *tmp++ = *self_p->data_register;
    }
    return bytes_read;
}

ssize32_t methodof(DeviceESCC, write)(class_t* self, const void* buf, size32_t count, size32_t offset)
{
    if ((ssize32_t)count < 0 || offset != 0) {
        return -1;
    }
    struct escc_struct* self_p = (struct escc_struct*)baseof(self, DeviceESCC);
    ssize32_t bytes_written = 0;
    const uint8_t* tmp = buf;
    while ((bytes_written++) < count) {
        *self_p->data_register = *tmp++;
        while (!(*self_p->cmd_register & 4)) {}
    }
    return bytes_written;
}
