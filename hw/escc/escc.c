#include <ehbc/hw/escc.h>
#include <ehbc/hw/device.h>
#include <ehbc/hw/bus.h>

int memberof(ESCC, construct)(void* _self, hwreg8_t* cmd_register, hwreg8_t* data_register)
{
    static const uint8_t init_sequence_8n1[] = {
        0x04, 0xC4,     /* no parity, 1 stop bit, clock x64 */
        0x0E, 0x04,     /* auto echo */
        0x03, 0xC1,     /* 8bpc for rx, enable rx */
        0x05, 0x68,     /* 8bpc for tx, enable tx */
    };
    ESCC* self = _self;
    self->cmd_register = cmd_register;
    self->data_register = data_register;

    for (int i = 0; i < sizeof(init_sequence_8n1); i++) {
        *self->cmd_register = init_sequence_8n1[i];
    }

    return 0;
}

ssize32_t memberof(ESCC, read)(void* _self, void* buf, size32_t count, size32_t offset)
{
    ESCC* self = _self;
    if ((ssize32_t)count < 0 || offset != 0) {
        return -1;
    }
    ssize32_t bytes_read = 0;
    uint8_t* tmp = buf;
    while (bytes_read < count && (*self->cmd_register & 1)) {
        bytes_read++;
        *tmp++ = *self->data_register;
    }
    return bytes_read;
}

ssize32_t memberof(ESCC, write)(void* _self, const void* buf, size32_t count, size32_t offset)
{
    ESCC* self = _self;
    if ((ssize32_t)count < 0 || offset != 0) {
        return -1;
    }
    ssize32_t bytes_written = 0;
    const uint8_t* tmp = buf;
    while ((bytes_written++) < count) {
        *self->data_register = *tmp++;
        while (!(*self->cmd_register & 4)) {}
    }
    return bytes_written;
}
