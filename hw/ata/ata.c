#include <ehbc/hw/ata.h>

#include <ehbc/hw/device.h>
#include <ehbc/hw/bus.h>
#include <ehbc/swap.h>
#include <ehbc/printf.h>

#include "hw/ata/ata_internal.h"

#define IOMODE_PIO  0
#define IOMODE_DMA  1

static inline uint16_t read_status(ATADrive* self)
{
    uint16_t result = self->reg_base[ATA_REG_STATUS];
    return result >> 8;
}

static inline void wait_busy(ATADrive* self)
{
    while (read_status(self) & 0x80) {}
}

static inline void wait_drdy(ATADrive* self)
{
    while (!(read_status(self) & 0x40)) {}
}

static inline void wait_drq(ATADrive* self)
{
    while (!(read_status(self) & 0x08)) {}
}

static inline int check_error(ATADrive* self)
{
    return (read_status(self) & 0x01) != 0;
}

static inline int check_drq(ATADrive* self)
{
    return (read_status(self) & 0x08) != 0;
}

static inline void send_command(ATADrive* self, uint8_t command)
{
    self->reg_base[ATA_REG_COMMAND] = command << 8;
}

static inline void write_data(ATADrive* self, uint16_t data)
{
    self->reg_base[ATA_REG_DATA] = data;
}

static inline uint16_t read_data(ATADrive* self)
{
    uint16_t data = self->reg_base[ATA_REG_DATA];
    return data;
}

int memberof(ATADrive, construct)(void* _self, hwreg16_t* reg_base)
{
    ATADrive* self = _self;
    self->reg_base = reg_base;

    wait_busy(self);
    wait_drdy(self);

    self->reg_base[ATA_REG_DRV_HEAD] = 0;
    send_command(self, 0xEC);

    wait_drq(self);

    for (int i = 0; i < 256; i++) {
        uint16_t data = read_data(self);
        ((uint16_t*)self->drive_ident.data)[i] = byteswap16(data);
    }

    while (check_drq(self)) {
        read_data(self);
    }

    return check_error(self);
}

const struct ata_drive_ident* memberof(ATADrive, get_identifier_struct)(void* _self)
{
    ATADrive* self = _self;
    return &self->drive_ident;
}

uint8_t memberof(ATADrive, read_sectors_pio)(void* _self, void* buf, uint32_t lba, uint8_t count)
{
    ATADrive* self = _self;
    uint8_t lba_frag0 = lba & 0xFF;
    uint8_t lba_frag1 = (lba >> 8) & 0xFF;
    uint8_t lba_frag2 = (lba >> 16) & 0xFF;
    uint8_t lba_frag3 = (lba >> 24) & 0xF;

    wait_busy(self);
    wait_drdy(self);

    self->reg_base[ATA_REG_SECT_COUNT] = count << 8;
    self->reg_base[ATA_REG_LBA_LOW] = lba_frag0 << 8;
    self->reg_base[ATA_REG_LBA_MID] = lba_frag1 << 8;
    self->reg_base[ATA_REG_LBA_HIGH] = lba_frag2 << 8;
    self->reg_base[ATA_REG_DRV_HEAD] = (lba_frag3 | 0x40) << 8;

    send_command(self, 0x20);

    for (int j = 0; j < count; j++) {
        wait_drq(self);

        for (int i = 0; i < 256; i++) {
            uint16_t data = read_data(self);
            ((uint16_t*)buf)[i + j * 256] = data;
        }
    }

    while (check_drq(self)) {
        read_data(self);
    }

    return check_error(self);
}

uint8_t memberof(ATADrive, write_sectors_pio)(void* _self, const void* buf, uint32_t lba, uint8_t count)
{
    ATADrive* self = _self;
    uint8_t lba_frag0 = lba & 0xFF;
    uint8_t lba_frag1 = (lba >> 8) & 0xFF;
    uint8_t lba_frag2 = (lba >> 16) & 0xFF;
    uint8_t lba_frag3 = (lba >> 24) & 0xF;

    wait_busy(self);
    wait_drdy(self);

    self->reg_base[ATA_REG_SECT_COUNT] = count << 8;
    self->reg_base[ATA_REG_LBA_LOW] = lba_frag0 << 8;
    self->reg_base[ATA_REG_LBA_MID] = lba_frag1 << 8;
    self->reg_base[ATA_REG_LBA_HIGH] = lba_frag2 << 8;
    self->reg_base[ATA_REG_DRV_HEAD] = (lba_frag3 | 0x40) << 8;

    send_command(self, 0x30);

    wait_drq(self);

    for (int i = 0; i < 256 * count; i++) {
        write_data(self, ((uint16_t*)buf)[i]);
    }

    return check_error(self);
}

ssize32_t memberof(ATADrive, read)(void* self, void* buf, size32_t count, size32_t offset)
{
    return memberof(ATADrive, read_sectors_pio)(self, buf, offset >> 9, count >> 9);
}

ssize32_t memberof(ATADrive, write)(void* self, const void* buf, size32_t count, size32_t offset)
{
    return memberof(ATADrive, write_sectors_pio)(self, buf, offset >> 9, count >> 9);
}
