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
#include <ehbc/elf.h>
#include <ehbc/stream.h>
#include <ehbc/encoding/utf8.h>

#define ESCC_CHA_CMD        ((uint8_t*)0xFF001201)
#define ESCC_CHA_DATA       ((uint8_t*)0xFF001203)

#define I8042_DATA          ((uint8_t*)0xFF001700)
#define I8042_CMD           ((uint8_t*)0xFF001701)

#define ATA0_MASTER_BASE    ((uint16_t*)0xFF001D00)
#define ATA0_SLAVE_BASE     ((uint16_t*)0xFF001D00)

void hexdump(const void* p, size_t len);

void main(void)
{
    ESCC escc;
    memberof(ESCC, construct)(&escc, ESCC_CHA_CMD, ESCC_CHA_DATA);

    set_io_device(&escc, &impl(ESCC, DeviceTrait));

    printf("%ld bytes used, %ld bytes free\r\n", get_used_stack_space(), get_free_stack_space());

    ATADrive ata;
    memberof(ATADrive, construct)(&ata, ATA0_MASTER_BASE);

    volatile long double fvar = 1.0f;
    fvar += 2.0f;

    FATFileSystem fatfs;
    memberof(FATFileSystem, construct)(&fatfs, &ata, &impl(ATADrive, DeviceTrait), 0);

    printf("%s\r\n", memberof(FATFileSystem, get_filesystem_name)(&fatfs));

    dir_t rootdir;
    memberof(FATFileSystem, open_directory)(&fatfs, NULL, &rootdir, FS_ROOT_DIR);

    dir_t subdir1;
    memberof(FATFileSystem, open_directory)(&fatfs, &rootdir, &subdir1, utf8("ASDF"));

    const fileinfo_t* fip = NULL;
    do {
        memberof(FATFileSystem, list_directory)(&fatfs, &rootdir, &fip);
        if (fip != NULL) {
            printf("::/%s\r\n", fip->name);
        }
    } while (fip != NULL);

    fip = NULL;
    do {
        memberof(FATFileSystem, list_directory)(&fatfs, &subdir1, &fip);
        if (fip != NULL) {
            printf("::/ASDF/%s\r\n", fip->name);
        }
    } while (fip != NULL);

    file_t file1;
    memberof(FATFileSystem, open_file)(&fatfs, &subdir1, &file1, utf8("filename.ext"), "r");

    char file_buf[16];
    memberof(FATFileSystem, read_file)(&fatfs, &file1, file_buf, 16, 1);
    printf("file_buf: %s\r\n", file_buf);

    file_t elffile;
    int ret = memberof(FATFileSystem, open_file)(&fatfs, &rootdir, &elffile, utf8("유니코드.elf"), "r");

    ELFObject elfobj;
    memberof(ELFObject, construct)(&elfobj, &elffile);

    memberof(ELFObject, shdr32) vector_header;
    memberof(ELFObject, get_section_header)(&elfobj, ".vector", &vector_header);
    hexdump(&vector_header, sizeof(vector_header));

    uint8_t elf_vector[16];
    memberof(ELFObject, get_section_data)(&elfobj, ".text", elf_vector, sizeof(elf_vector));
    hexdump(elf_vector, sizeof(elf_vector));

    memberof(ELFObject, destruct)(&elfobj);
    memberof(FATFileSystem, close_file)(&fatfs, &elffile);
    memberof(FATFileSystem, close_file)(&fatfs, &file1);
    memberof(FATFileSystem, close_directory)(&fatfs, &subdir1);
    memberof(FATFileSystem, close_directory)(&fatfs, &rootdir);

    Stream stream;
    memberof(Stream, construct)(&stream, sizeof(char), 4);

    memberof(Stream, push_data)(&stream, "a");
    memberof(Stream, push_data)(&stream, "b");
    memberof(Stream, push_data)(&stream, "c");
    memberof(Stream, push_data)(&stream, "d");
    memberof(Stream, push_data)(&stream, "e");

    char char_buf;
    memberof(Stream, pop_data)(&stream, &char_buf);
    printf("%c\r\n", char_buf);
    memberof(Stream, pop_data)(&stream, &char_buf);
    printf("%c\r\n", char_buf);
    memberof(Stream, pop_data)(&stream, &char_buf);
    printf("%c\r\n", char_buf);
    memberof(Stream, pop_data)(&stream, &char_buf);
    printf("%c\r\n", char_buf);

    printf("%d\r\n", memberof(Stream, is_available)(&stream));

    memberof(Stream, destruct)(&stream);

    // Assuming that the size of the terminal is 80x25
    // printf("\033[0;0H\033[=18h\033[2J\033[47;94m\033[K\033[34Gehbc-firmware\033[E");


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
        int read_count = memberof(ESCC, read)(&escc, escc_buffer, sizeof(escc_buffer), 0);
        for (int i = 0; i < read_count; i++) {
            switch (escc_buffer[i]) {
                case '\r':
                case '\n':
                    printf("\r\nbuf> %s\r\n", input_buffer);
                    ibuf_cur = 0;
                    input_buffer[ibuf_cur] = 0;
                    break;
                case '\x7F':
                    if (ibuf_cur > 0) {
                        ibuf_cur--;
                        input_buffer[ibuf_cur] = 0;
                        const char bksp_seq[] = "\033[1D\033[K";
                        memberof(ESCC, write)(&escc, bksp_seq, 7, 0);
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
}
