#include <ehbc/hw/ata.h>

#include <ehbc/hw/device.h>
#include <ehbc/hw/bus.h>
#include <ehbc/swap.h>
#include <ehbc/printf.h>

#include "hw/ata/ata_internal.h"

#define IOMODE_PIO  0
#define IOMODE_DMA  1

struct ata_struct {
    hwreg16_t*  reg_base;
    uint16_t    drive_ident[512];
    uint16_t    io_mode;
};

static void ata_constructor(class_t* self)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);
    self_p->io_mode = IOMODE_PIO;
    Device->constructor(self);
}

static void ata_destructor(class_t* self)
{
    Device->destructor(self);
}

static ssize32_t read_func_wrapper(class_t* self, void* buf, size32_t count, size32_t offset)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);

    if ((count & 511) != 0 || (offset & 511) != 0) {
        return -1;
    }

    switch (self_p->io_mode) {
        case IOMODE_PIO:
            return methodof(DeviceATA, read_sectors_pio)(self, buf, offset >> 9, count >> 9);
            break;
        case IOMODE_DMA:
        default:
            return -1;
    }
}

static ssize32_t write_func_wrapper(class_t* self, const void* buf, size32_t count, size32_t offset)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);

    if ((count & 511) != 0 || (offset & 511) != 0) {
        return -1;
    }

    switch (self_p->io_mode) {
        case IOMODE_PIO:
            return methodof(DeviceATA, write_sectors_pio)(self, buf, offset >> 9, count >> 9);
            break;
        case IOMODE_DMA:
        default:
            return -1;
    }
}

static const struct _device_vtable vtbl = {
    .read = read_func_wrapper,
    .write = write_func_wrapper,
};

static const struct class_parent_entry ata_parents[] = {
    { Device, sizeof(struct ata_struct), &vtbl }
};

static classid_t _device_ata_def_id;

const classdef_t _device_ata_def = {
    .id = &_device_ata_def_id,
    .name = "DeviceATA",
    .constructor = ata_constructor,
    .destructor = ata_destructor,
    .parents = ata_parents,
    .parents_count = 1,
    .data_size = sizeof(struct ata_struct),
};

__attribute__((constructor))
static void __class_def_DeviceATA(void)
{
    _device_ata_def_id = ++__next_class_id;
}

static inline uint16_t read_status(struct ata_struct* self_p)
{
    uint16_t result = self_p->reg_base[ATA_REG_STATUS];
    return result >> 8;
}

static inline void wait_busy(struct ata_struct* self_p)
{
    while (read_status(self_p) & 0x80) {}
}

static inline void wait_drdy(struct ata_struct* self_p)
{
    while (!(read_status(self_p) & 0x40)) {}
}

static inline void wait_drq(struct ata_struct* self_p)
{
    while (!(read_status(self_p) & 0x08)) {}
}

static inline int check_error(struct ata_struct* self_p)
{
    return (read_status(self_p) & 0x01) != 0;
}

static inline int check_drq(struct ata_struct* self_p)
{
    return (read_status(self_p) & 0x08) != 0;
}

static inline void send_command(struct ata_struct* self_p, uint8_t command)
{
    self_p->reg_base[ATA_REG_COMMAND] = command << 8;
}

static inline void write_data(struct ata_struct* self_p, uint16_t data)
{
    self_p->reg_base[ATA_REG_DATA] = data;
}

static inline uint16_t read_data(struct ata_struct* self_p)
{
    uint16_t data = self_p->reg_base[ATA_REG_DATA];
    return data;
}

int methodof(DeviceATA, initialize)(class_t* self, hwreg16_t* reg_base)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);
    self_p->reg_base = reg_base;

    wait_busy(self_p);
    wait_drdy(self_p);

    self_p->reg_base[ATA_REG_DRV_HEAD] = 0;
    send_command(self_p, 0xEC);

    wait_drq(self_p);

    for (int i = 0; i < 256; i++) {
        uint16_t data = read_data(self_p);
        self_p->drive_ident[i] = byteswap16(data);
    }

    while (check_drq(self_p)) {
        read_data(self_p);
    }

    return check_error(self_p);
}

void methodof(DeviceATA, get_identifier_struct)(class_t* self, void* struct_buf)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);
    for (int i = 0; i < 256; i++) {
        ((uint16_t*)struct_buf)[i] = self_p->drive_ident[i];
    }
}

int methodof(DeviceATA, read_sectors_pio)(class_t* self, void* buf, uint32_t lba, uint8_t count)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);

    uint8_t lba_frag0 = lba & 0xFF;
    uint8_t lba_frag1 = (lba >> 8) & 0xFF;
    uint8_t lba_frag2 = (lba >> 16) & 0xFF;
    uint8_t lba_frag3 = (lba >> 24) & 0xF;

    wait_busy(self_p);
    wait_drdy(self_p);

    self_p->reg_base[ATA_REG_SECT_COUNT] = count << 8;
    self_p->reg_base[ATA_REG_LBA_LOW] = lba_frag0 << 8;
    self_p->reg_base[ATA_REG_LBA_MID] = lba_frag1 << 8;
    self_p->reg_base[ATA_REG_LBA_HIGH] = lba_frag2 << 8;
    self_p->reg_base[ATA_REG_DRV_HEAD] = (lba_frag3 | 0x40) << 8;

    send_command(self_p, 0x20);

    wait_drq(self_p);

    for (int i = 0; i < 256 * count; i++) {
        uint16_t data = read_data(self_p);
        ((uint16_t*)buf)[i] = data;
    }

    while (check_drq(self_p)) {
        read_data(self_p);
    }

    return check_error(self_p);
}

int methodof(DeviceATA, write_sectors_pio)(class_t* self, const void* buf, uint32_t lba, uint8_t count)
{
    struct ata_struct* self_p = (struct ata_struct*)baseof(self, DeviceATA);

    uint8_t lba_frag0 = lba & 0xFF;
    uint8_t lba_frag1 = (lba >> 8) & 0xFF;
    uint8_t lba_frag2 = (lba >> 16) & 0xFF;
    uint8_t lba_frag3 = (lba >> 24) & 0xF;

    wait_busy(self_p);
    wait_drdy(self_p);

    self_p->reg_base[ATA_REG_SECT_COUNT] = count << 8;
    self_p->reg_base[ATA_REG_LBA_LOW] = lba_frag0 << 8;
    self_p->reg_base[ATA_REG_LBA_MID] = lba_frag1 << 8;
    self_p->reg_base[ATA_REG_LBA_HIGH] = lba_frag2 << 8;
    self_p->reg_base[ATA_REG_DRV_HEAD] = (lba_frag3 | 0x40) << 8;

    send_command(self_p, 0x30);

    wait_drq(self_p);

    for (int i = 0; i < 256 * count; i++) {
        write_data(self_p, ((uint16_t*)buf)[i]);
    }

    return check_error(self_p);
}
