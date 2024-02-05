#ifndef _EHBC_HW_ATA_H__
#define _EHBC_HW_ATA_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/hw/device.h>

struct ata_drive_ident {
    uint8_t data[512];
};

typedef struct {
    hwreg16_t*  reg_base;
    struct ata_drive_ident drive_ident;
    uint16_t    io_mode;
} ATADrive;

int memberof(ATADrive, construct)(void* self, hwreg16_t* reg_base);
const struct ata_drive_ident* memberof(ATADrive, get_identifier_struct)(void* self);
uint8_t memberof(ATADrive, read_sectors_pio)(void* self, void* buf, uint32_t lba, uint8_t count);
uint8_t memberof(ATADrive, write_sectors_pio)(void* self, const void* buf, uint32_t lba, uint8_t count);
ssize32_t memberof(ATADrive, read)(void* self, void* buf, size32_t count, size32_t offset);
ssize32_t memberof(ATADrive, write)(void* self, const void* buf, size32_t count, size32_t offset);

static const struct {
    int (*construct)(void* self, hwreg16_t* reg_base);
    const struct ata_drive_ident* (*get_identifier_struct)(void* self);
    uint8_t (*read_sectors_pio)(void* self, void* buf, uint32_t lba, uint8_t count);
    uint8_t (*write_sectors_pio)(void* self, const void* buf, uint32_t lba, uint8_t count);

    DeviceTrait impl(DeviceTrait);
} ftableof(ATADrive) = {
    .construct = memberof(ATADrive, construct),
    .get_identifier_struct = memberof(ATADrive, get_identifier_struct),
    .read_sectors_pio = memberof(ATADrive, read_sectors_pio),
    .write_sectors_pio = memberof(ATADrive, write_sectors_pio),

    .impl(DeviceTrait) = {
        .read = memberof(ATADrive, read),
        .write = memberof(ATADrive, write),
    }
};

#endif  // _EHBC_HW_ATA_H__
