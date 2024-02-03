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
#include <ehbc/string.h>

#define ESCC_CHA_CMD        ((uint8_t*)0xFF001201)
#define ESCC_CHA_DATA       ((uint8_t*)0xFF001203)

#define I8042_DATA          ((uint8_t*)0xFF001700)
#define I8042_CMD           ((uint8_t*)0xFF001701)

#define ATA0_MASTER_BASE    ((uint16_t*)0xFF001D00)
#define ATA0_SLAVE_BASE     ((uint16_t*)0xFF001D00)


void main(void)
{
    ESCC escc;
    methodof(ESCC, construct)(&escc, ESCC_CHA_CMD, ESCC_CHA_DATA);

    set_io_device(&escc, &ftableof(ESCC).impl(DeviceTrait));
    printf("\r\n");

    printf("%ld bytes used, %ld bytes free\r\n", get_used_heap_space() + get_used_stack_space(), get_free_stack_space());

    ATADrive ata;
    methodof(ATADrive, construct)(&ata, ATA0_MASTER_BASE);

    printf("%ld bytes used, %ld bytes free\r\n", get_used_heap_space() + get_used_stack_space(), get_free_stack_space());

    printf("debug: Initialized debugging environment\r\n");

    printf("sizeof(char) = %lu\r\n", sizeof(char));
    printf("sizeof(short) = %lu\r\n", sizeof(short));
    printf("sizeof(int) = %lu\r\n", sizeof(int));
    printf("sizeof(long) = %lu\r\n", sizeof(long));
    printf("sizeof(long long) = %lu\r\n", sizeof(long long));
    printf("sizeof(float) = %lu\r\n", sizeof(float));
    printf("sizeof(double) = %lu\r\n", sizeof(double));
    printf("sizeof(long double) = %lu\r\n", sizeof(long double));
    printf("sizeof(intmax_t) = %lu\r\n", sizeof(intmax_t));
    printf("sizeof(void*) = %lu\r\n", sizeof(void*));

    volatile long double fvar = 1.0f;
    fvar += 2.0f;

    printf("%ld bytes used, %ld bytes free\r\n", get_used_heap_space() + get_used_stack_space(), get_free_stack_space());

    FATFileSystem fatfs;
    methodof(FATFileSystem, construct)(&fatfs, &ata, &ftableof(ATADrive).impl(DeviceTrait), 0);

    printf("%ld bytes used, %ld bytes free\r\n", get_used_heap_space() + get_used_stack_space(), get_free_stack_space());

    printf("%s\r\n", methodof(FATFileSystem, get_filesystem_name)(&fatfs));

    dir_t rootdir;
    methodof(FATFileSystem, open_directory)(&fatfs, NULL, &rootdir, FS_ROOT_DIR);

    dir_t subdir1;
    methodof(FATFileSystem, open_directory)(&fatfs, &rootdir, &subdir1, "ASDF");

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

    char escc_buffer[16];
    char input_buffer[512];
    int ibuf_cur = 0;
    while (1) {
        /*
        uint8_t i8042_status = *I8042_CMD;
        if (i8042_status & 0x1) {
            printf("i8042 read: %02x\r\n", *I8042_DATA);
        }
        */
        int read_count = methodof(ESCC, read)(&escc, escc_buffer, sizeof(escc_buffer), 0);
        for (int i = 0; i < read_count; i++) {
            switch (escc_buffer[i]) {
                case '\r':
                case '\n':
                    printf("\r\n");
                    if (strncmp(input_buffer, "dir", 4) == 0) {
                        const fileinfo_t* fip = NULL;
                        do {
                            methodof(FATFileSystem, list_directory)(&fatfs, &rootdir, &fip);
                            if (fip != NULL) {
                                printf("%s\r\n", fip->name);
                            }
                        } while (fip != NULL);
                    } else if (strncmp(input_buffer, "dir2", 5) == 0) {
                        const fileinfo_t* fip = NULL;
                        do {
                            methodof(FATFileSystem, list_directory)(&fatfs, &subdir1, &fip);
                            if (fip != NULL) {
                                printf("%s\r\n", fip->name);
                            }
                        } while (fip != NULL);
                    } else if (strncmp(input_buffer, "find ", 5) == 0) {
                        const fileinfo_t* fip = NULL;
                        methodof(FATFileSystem, match_name)(&fatfs, &rootdir, input_buffer + 5, &fip);
                        if (fip != NULL) {
                            printf("File found: %s\r\n", fip->name);
                        } else {
                            printf("File not found: %s\r\n", input_buffer + 5);
                        }
                    }
                    ibuf_cur = 0;
                    break;
                case '\x7F':
                    if (ibuf_cur > 0) {
                        ibuf_cur--;
                        input_buffer[ibuf_cur] = 0;
                        const char bksp_seq[] = "\b\0\b";
                        methodof(ESCC, write)(&escc, bksp_seq, 3, 0);
                    }
                    break;
                default:
                    printf("%c", escc_buffer[i]);
                    input_buffer[ibuf_cur++] = escc_buffer[i];
                    input_buffer[ibuf_cur] = 0;
                    break;
            }
        }
    }
    methodof(FATFileSystem, close_directory)(&fatfs, &subdir1);
    methodof(FATFileSystem, close_directory)(&fatfs, &rootdir);
}
