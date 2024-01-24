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
#include <ehbc/format.h>
#include <ehbc/swap.h>
#include <ehbc/hw/escc.h>

#define ESCC_CHA_CMD        ((uint8_t*)0xFF101201)
#define ESCC_CHA_DATA       ((uint8_t*)0xFF101203)

#define I8042_DATA          ((uint8_t*)0xFF101700)
#define I8042_CMD           ((uint8_t*)0xFF101701)

#define ATA0_MASTER_BASE    ((uint16_t*)0xFF101D00)
#define ATA0_SLAVE_BASE     ((uint16_t*)0xFF101D00)

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

static const uint8_t init_sequence_8n1[] = {
    0x04, 0xC4,     /* no parity, 1 stop bit, clock x64 */
    0x0E, 0x04,     /* auto echo */
    0x03, 0xC1,     /* 8bpc for rx, enable rx */
    0x05, 0x68,     /* 8bpc for tx, enable tx */
};

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
    methodof(DeviceESCC, initialize_channel_a)(escc, ESCC_CHA_CMD, ESCC_CHA_DATA);

    methodof(DeviceESCC, print_channel_a)(escc, "debug: Initialized debugging environment\r\n");
    size32_t stack_size_kb = get_stack_free_space() / 1024;




    /* format string */
    char str_buf[6];

    str_buf[0] = stack_size_kb / 10000 + '0';
    stack_size_kb %= 10000;
    str_buf[1] = stack_size_kb / 1000  + '0';
    stack_size_kb %= 1000;
    str_buf[2] = stack_size_kb / 100   + '0';
    stack_size_kb %= 100;
    str_buf[3] = stack_size_kb / 10    + '0';
    stack_size_kb %= 10;
    str_buf[4] = stack_size_kb         + '0';
    methodof(DeviceESCC, print_channel_a)(escc, str_buf);
    methodof(DeviceESCC, print_channel_a)(escc, "kiB Free\r\n");

    char str_buf2[128];
    format(str_buf2, sizeof(str_buf2), "ata0_master status: %04x\r\n", ATA0_MASTER_BASE[ATA_REG_STATUS]);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);

    ata_send_command(ATA0_MASTER_BASE, 0x90);
    methodof(DeviceESCC, print_channel_a)(escc, "ata0_master diagnostics:");
    for (int i = 1; i < 8; i++) {
        format(str_buf2, sizeof(str_buf2), "%02x ", byteswap16(ATA0_MASTER_BASE[i]));
        methodof(DeviceESCC, print_channel_a)(escc, str_buf2);
    }
    methodof(DeviceESCC, print_channel_a)(escc, "\r\n");

    format(str_buf2, sizeof(str_buf2), "i8042 status: %02x\r\n", *I8042_CMD);

    *I8042_CMD = 0xA4;
    format(str_buf2, sizeof(str_buf2), "i8042 check password: %02x\r\n", *I8042_DATA);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);
    *I8042_CMD = 0xA9;
    format(str_buf2, sizeof(str_buf2), "i8042 mouse test: %02x\r\n", *I8042_DATA);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);
    *I8042_CMD = 0xAB;
    format(str_buf2, sizeof(str_buf2), "i8042 keyboard test: %02x\r\n", *I8042_DATA);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);
    *I8042_CMD = 0xAA;
    format(str_buf2, sizeof(str_buf2), "i8042 self test: %02x\r\n", *I8042_DATA);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);
    *I8042_CMD = 0x20;
    format(str_buf2, sizeof(str_buf2), "i8042 command register: %02x\r\n", *I8042_DATA);
    methodof(DeviceESCC, print_channel_a)(escc, str_buf2);

    while (1) {
        /*
        uint8_t i8042_status = *I8042_CMD;
        if (i8042_status & 0x1) {
            format(str_buf2, sizeof(str_buf2), "i8042 read: %02x\r\n", *I8042_DATA);
            debug_print(str_buf2);
        }
        */

        if (debug_poll_input()) {  // character available
            char input_char = *ESCC_CHA_DATA;
            switch (input_char) {
                case '\r':
                    methodof(DeviceESCC, print_channel_a)(escc, "\r\n");
                    break;
                default:
                    break;
            }
        }
    }


    delete_class(escc);
}
