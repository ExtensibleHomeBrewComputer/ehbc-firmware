#include "fs/fat/fat_internal.h"

#include <ehbc/fs/fat.h>
#include <ehbc/hw/device.h>
#include <ehbc/swap.h>
#include <ehbc/printf.h>

#define FAT_TYPE_UNKNOWN    0
#define FAT_TYPE_FAT12      1
#define FAT_TYPE_FAT16      2
#define FAT_TYPE_FAT32      3

struct fat_struct {
    class_t* blkdev;

    void*    sector_buf;
    void*    fat;

    uint16_t reserved_sector_count;
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint32_t data_area_begin;
    uint32_t fat_size;
    uint32_t free_clusters;
    uint32_t next_free_cluster;
    uint32_t total_sector_count;
    uint32_t root_cluster;
    uint16_t root_entry_count;
    uint16_t initialized : 1;
    uint16_t fat_type    : 2;
};

static void fat_constructor(class_t* self)
{
    Device->constructor(self);
    struct fat_struct* self_p = (struct fat_struct*)baseof(self, FileSystemFAT);
    self_p->initialized = 0;
    self_p->fat_type = FAT_TYPE_UNKNOWN;
}

static void fat_destructor(class_t* self)
{
    struct fat_struct* self_p = (struct fat_struct*)baseof(self, FileSystemFAT);
    if (self_p->initialized) {
        mem_free(self_p->fat);
        mem_free(self_p->sector_buf);
    }
    Device->destructor(self);
}

static classid_t _fs_fat_def_id;

const classdef_t _fs_fat_def = {
    .id = &_fs_fat_def_id,
    .name = "FileSystemFAT",
    .constructor = fat_constructor,
    .destructor = fat_destructor,
    .parents = NULL,
    .parents_count = 0,
    .data_size = sizeof(struct fat_struct),
};

__attribute__((constructor))
static void __class_def_FileSystemFAT(void)
{
    _fs_fat_def_id = ++__next_class_id;
}

static int read_sector(struct fat_struct* self_p, void* buf, uint32_t sector, uint32_t count)
{
    return methodof(Device, read)(
        self_p->blkdev,
        buf,
        count * self_p->bytes_per_sector,
        sector * self_p->bytes_per_sector);
}

static uint16_t get_next_cluster12(struct fat_struct* self_p, uint16_t current)
{
    uint16_t cluster_pair_idx = (current >> 1) * 3;
    uint8_t* fat = self_p->fat;
    if (current & 1) {  // odd-numbered cluster
        return ((fat[cluster_pair_idx + 1] & 0xF0) >> 4) | (fat[cluster_pair_idx + 2] << 4);
    } else {  // even-numbered cluster
        return fat[cluster_pair_idx] + ((fat[cluster_pair_idx + 1] & 0x0F) << 8);
    }
}

static uint16_t get_next_cluster16(struct fat_struct* self_p, uint16_t current)
{
    return byteswap16(((uint16_t*)self_p->fat)[current]);
}

static uint32_t get_next_cluster32(struct fat_struct* self_p, uint32_t current)
{
    return byteswap32(((uint32_t*)self_p->fat)[current]);
}

static uint16_t sector_to_cluster12(struct fat_struct* self_p, uint16_t sector)
{
    return (sector - self_p->data_area_begin) / self_p->data_area_begin;
}

static uint16_t sector_to_cluster16(struct fat_struct* self_p, uint16_t sector)
{
    return (sector - self_p->data_area_begin) / self_p->data_area_begin;
}

static uint32_t sector_to_cluster32(struct fat_struct* self_p, uint32_t sector)
{
    return (sector - self_p->data_area_begin) / self_p->data_area_begin;
}

static uint16_t cluster_to_sector12(struct fat_struct* self_p, uint16_t cluster)
{
    return (cluster * self_p->sectors_per_cluster) + self_p->data_area_begin;
}

static uint16_t cluster_to_sector16(struct fat_struct* self_p, uint16_t cluster)
{
    return (cluster * self_p->sectors_per_cluster) + self_p->data_area_begin;
}

static uint32_t cluster_to_sector32(struct fat_struct* self_p, uint32_t cluster)
{
    return (cluster * self_p->sectors_per_cluster) + self_p->data_area_begin;
}

static uint16_t get_next_sector12(struct fat_struct* self_p, uint16_t current)
{
    current -= self_p->data_area_begin;

    if (current % self_p->sectors_per_cluster != self_p->sectors_per_cluster - 1) {
        return self_p->data_area_begin + current + 1;
    }

    current /= self_p->sectors_per_cluster;
    uint16_t next_cluster = get_next_cluster12(self_p, current);

    return cluster_to_sector12(self_p, next_cluster);
}

static uint16_t get_next_sector16(struct fat_struct* self_p, uint16_t current)
{
    current -= self_p->data_area_begin;

    if (current % self_p->sectors_per_cluster != self_p->sectors_per_cluster - 1) {
        return self_p->data_area_begin + current + 1;
    }

    current /= self_p->sectors_per_cluster;
    uint16_t next_cluster = get_next_cluster16(self_p, current);

    return cluster_to_sector16(self_p, next_cluster);
}

static uint32_t get_next_sector32(struct fat_struct* self_p, uint32_t current)
{
    current -= self_p->data_area_begin;

    if (current % self_p->sectors_per_cluster != self_p->sectors_per_cluster - 1) {
        return self_p->data_area_begin + current + 1;
    }

    current /= self_p->sectors_per_cluster;
    uint32_t next_cluster = get_next_cluster32(self_p, current);

    return cluster_to_sector32(self_p, next_cluster);
}

#define is_lfnentry(entry) (((entry)->file.attribute & FAT_ATTR_LFNENTRY) == FAT_ATTR_LFNENTRY)

static size32_t get_lfn_filename(const union fat_dir_entry* entry, char buf[static FAT_LFN_LENGTH])
{
    /* TODO: UCS-2 Unicode support */
    if (is_lfnentry(entry)) {
        buf += ((entry->lfn.sequence_index & 0x1F) - 1) * 13;
        size32_t char_count = 0;
        for (int i = 0; i < 5; i++) {
            buf[char_count++] = entry->lfn.name_fragment1[i] >> 8;
        }
        for (int i = 0; i < 6; i++) {
            buf[char_count++] = entry->lfn.name_fragment2[i] >> 8;
        }
        for (int i = 0; i < 2; i++) {
            buf[char_count++] = entry->lfn.name_fragment3[i] >> 8;
        }
        if (entry->lfn.sequence_index & 0x40) {
            buf[char_count] = '\0';
        }
        return char_count;
    } else {
        return 0;
    }
}

static size32_t get_sfn_filename(const union fat_dir_entry* entry, char buf[static FAT_SFN_LENGTH])
{
    if (!is_lfnentry(entry)) {
        size32_t char_count = 0;
        for (int i = 0; i < 8 && entry->file.name[i] != ' '; i++) {
            buf[char_count++] = entry->file.name[i];
        }
        if (entry->file.extension[0] != ' ') {
            buf[char_count++] = '.';
        }
        for (int i = 0; i < 3 && entry->file.extension[i] != ' '; i++) {
            buf[char_count++] = entry->file.extension[i];
        }
        buf[char_count] = '\0';
        return char_count;
    } else {
        return 0;
    }
}

static uint8_t get_sfn_checksum(char buf[static FAT_SFN_LENGTH])
{
    uint8_t chksum = 0;
    for (int i = FAT_SFN_LENGTH; i != 0; i--) {
        chksum = ((chksum & 1) ? 0x80 : 0) + (chksum >> 1) + *buf++;
    }

    return chksum;
}

int methodof(FileSystemFAT, initialize_from_blkdev)(class_t* self, class_t* blkdev)
{
    struct fat_struct* self_p = (struct fat_struct*)baseof(self, FileSystemFAT);

    self_p->blkdev = blkdev;

    self_p->sector_buf = mem_alloc(512);
    self_p->bytes_per_sector = 512;

    // Read sector 0 (BPB)
    read_sector(self_p, self_p->sector_buf, 0, 1);

    const struct fat_common_bpb* bpb = self_p->sector_buf;
    const struct fat1216_bpb* bpb1216 = self_p->sector_buf;
    const struct fat32_bpb* bpb32 = self_p->sector_buf;

    self_p->sectors_per_cluster = bpb->sectors_per_cluster;
    self_p->bytes_per_sector = byteswap16(bpb->bytes_per_sector);
    self_p->root_entry_count = byteswap16(bpb->root_entry_count);
    self_p->reserved_sector_count = byteswap16(bpb->reserved_sector_count);

    // Determine FAT type
    uint16_t rootdir_sectors = ((self_p->root_entry_count * 32) + (self_p->bytes_per_sector - 1)) / self_p->bytes_per_sector;

    if (bpb->fat_size16 != 0) {
        self_p->fat_size = byteswap16(bpb->fat_size16);
    } else {
        self_p->fat_size = byteswap32(bpb32->fat_size32);
    }

    if (bpb->total_sector_count16 != 0) {
        self_p->total_sector_count = byteswap16(bpb->total_sector_count16);
    } else {
        self_p->total_sector_count = byteswap32(bpb->total_sector_count32);
    }

    self_p->data_area_begin = self_p->reserved_sector_count + (bpb->fat_count * self_p->fat_size);
    uint32_t data_sectors = self_p->total_sector_count - (self_p->data_area_begin + rootdir_sectors);
    uint32_t cluster_count = data_sectors / self_p->sectors_per_cluster;

    if (cluster_count < 4085) {
        self_p->fat_type = FAT_TYPE_FAT12;
    } else if (cluster_count < 65525) {
        self_p->fat_type = FAT_TYPE_FAT16;
    } else {
        self_p->fat_type = FAT_TYPE_FAT32;
    }

    // Read FAT
    self_p->fat = mem_alloc(self_p->fat_size * self_p->bytes_per_sector);
    read_sector(self_p, self_p->fat, self_p->reserved_sector_count, 1);  // Fix it later

    self_p->initialized = 1;

    if (self_p->fat_type == FAT_TYPE_FAT32) {
        self_p->root_cluster = bpb32->root_cluster;

        read_sector(self_p, self_p->sector_buf, 1, 1);
        const struct fat_fsinfo* fsinfo = self_p->sector_buf;

        self_p->free_clusters = fsinfo->free_clusters;
        self_p->next_free_cluster = fsinfo->next_free_cluster;
    }

    return 0;
}

const char* methodof(FileSystemFAT, get_filesystem_name)(class_t* self)
{
    static const char* const fat_name[] = {
        "UNKNOWN",
        "FAT12",
        "FAT16",
        "FAT32",
    };

    struct fat_struct* self_p = (struct fat_struct*)baseof(self, FileSystemFAT);

    return fat_name[self_p->fat_type];
}

int methodof(FileSystemFAT, list_directory)(class_t* self, const char* path, char** buf, uint32_t count)
{
    struct fat_struct* self_p = (struct fat_struct*)baseof(self, FileSystemFAT);
    if (!self_p->initialized) {
        return -1;
    }

    // Root path
    union fat_dir_entry* entries = self_p->sector_buf;
    uint32_t current_sector;

    if (self_p->fat_type == FAT_TYPE_FAT32) {
        current_sector = cluster_to_sector32(self_p, self_p->root_cluster);
    } else {
        current_sector = self_p->data_area_begin;
    }

    int idx = 0;
    int is_lfn = 0;
    char filename_buf[FAT_LFN_LENGTH];

    while (1) {
        read_sector(self_p, self_p->sector_buf, current_sector, 1);

        for (int i = 0; i < self_p->bytes_per_sector / sizeof(union fat_dir_entry); i++) {
            if (!entries[i].file.name[0]) {
                goto finish;
            } else if (is_lfnentry(&entries[i])) {
                get_lfn_filename(&entries[i], filename_buf);
                is_lfn = 1;
            } else if (is_lfn) {
                printf("lfn: %s\r\n", filename_buf);
                is_lfn = 0;
            } else {
                get_sfn_filename(&entries[i], filename_buf);
                printf("sfn: %s\r\n", filename_buf);
            }
        }

        if (self_p->fat_type == FAT_TYPE_FAT32) {
            current_sector = get_next_sector32(self_p, current_sector);
        } else if (idx < self_p->root_entry_count) {
            current_sector++;
        } else {
            break;
        }
    }

finish:
    return 0;
}
