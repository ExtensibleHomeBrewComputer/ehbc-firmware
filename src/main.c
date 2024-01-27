/**
 * @file start.c
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Firmware entry point
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include <ehbc/types.h>
#include <ehbc/stack.h>
#include <ehbc/printf.h>
#include <ehbc/swap.h>
#include <ehbc/hw/escc.h>
#include <ehbc/hw/ata.h>
#include <ehbc/fs/fat.h>

#define ESCC_CHA_CMD        ((uint8_t*)0xFF001201)
#define ESCC_CHA_DATA       ((uint8_t*)0xFF001203)

#define I8042_DATA          ((uint8_t*)0xFF001700)
#define I8042_CMD           ((uint8_t*)0xFF001701)

#define ATA0_MASTER_BASE    ((uint16_t*)0xFF001D00)
#define ATA0_SLAVE_BASE     ((uint16_t*)0xFF001D00)

#define ATA_REG_DATA        0
#define ATA_REG_ERROR       1
#define ATA_REG_FEATURES    1
#define ATA_REG_SECT_COUNT  2
#define ATA_REG_SECT_NUM    3
#define ATA_REG_CYL_LOW     4
#define ATA_REG_CYL_HIGH    5
#define ATA_REG_DRV_HEAD    6
#define ATA_REG_LBA_LOW     3
#define ATA_REG_LBA_MID     4
#define ATA_REG_LBA_HIGH    5
#define ATA_REG_STATUS      7
#define ATA_REG_COMMAND     7

void hexdump(const void* p, size_t len);

static inline int debug_poll_input(void)
{
    return *ESCC_CHA_CMD & 1;
}

static inline void ata_send_command(uint16_t* ata_mmio_base, uint8_t command)
{
    ata_mmio_base[ATA_REG_COMMAND] = byteswap16(command);
}

void main(void)
{
    class_t* escc = new_class(DeviceESCC);
    methodof(DeviceESCC, initialize)(escc, ESCC_CHA_CMD, ESCC_CHA_DATA);

    set_io_device(escc);
    printf("\r\n");

    class_t* ata = new_class(DeviceATA);
    methodof(DeviceATA, initialize)(ata, ATA0_MASTER_BASE);

    printf("debug: Initialized debugging environment\r\n");

    printf("sizeof(intmax_t) = %lu\r\n", sizeof(intmax_t));

    printf("sizeof(long) = %lu\r\n", sizeof(long));

    printf("sizeof(long long) = %lu\r\n", sizeof(long long));

    printf("%ld bytes free\r\n", get_stack_free_space());

    class_t* fatfs = new_class(FileSystemFAT);
    methodof(FileSystemFAT, initialize_from_blkdev)(fatfs, ata);

    printf("%ld bytes free\r\n", get_stack_free_space());

    printf("%s\r\n", methodof(FileSystemFAT, get_filesystem_name)(fatfs));

    methodof(FileSystemFAT, list_directory)(fatfs, "", NULL, 0);


    /*
    printf("i8042 status: %02x\r\n", *I8042_CMD);

    *I8042_CMD = 0xA4;
    printf("\033[31;1;4mi8042 check password: %02x\033[0m\r\n", *I8042_DATA);

    *I8042_CMD = 0xA9;
    printf("i8042 mouse test: %02x\r\n", *I8042_DATA);

    *I8042_CMD = 0xAB;
    printf("i8042 keyboard test: %02x\r\n", *I8042_DATA);

    *I8042_CMD = 0xAA;
    printf("i8042 self test: %02x\r\n", *I8042_DATA);

    *I8042_CMD = 0x20;
    printf("i8042 command register: %02x\r\n", *I8042_DATA);
    */

    while (1) {
        /*
        uint8_t i8042_status = *I8042_CMD;
        if (i8042_status & 0x1) {
            printf("i8042 read: %02x\r\n", *I8042_DATA);
        }
        */
        if (debug_poll_input()) {  // character available
            char input_char = *ESCC_CHA_DATA;
            switch (input_char) {
                case '\r':
                    printf("\r\n");
                    break;
                default:
                    printf("%c", input_char);
                    break;
            }
        }
    }

    delete_class(fatfs);
    delete_class(ata);
    delete_class(escc);
}
