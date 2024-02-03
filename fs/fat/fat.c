#include "fs/fat/fat_internal.h"

#include <ehbc/fs/fat.h>
#include <ehbc/hw/device.h>
#include <ehbc/swap.h>
#include <ehbc/printf.h>
#include <ehbc/string.h>

#define FAT_TYPE_UNKNOWN    0
#define FAT_TYPE_FAT12      1
#define FAT_TYPE_FAT16      2
#define FAT_TYPE_FAT32      3

#define file_data(file) ((struct file_t_data*)file->data)
#define dir_data(dir) ((struct dir_t_data*)dir->data)
#define fileinfo_data(finfo) ((union fat_dir_entry*)finfo->data)
#define is_lfnentry(entry) (((entry)->file.attribute & FAT_ATTR_LFNENTRY) == FAT_ATTR_LFNENTRY)
#define is_root(dir) (dir_data(dir)->head_cluster == 0)

struct file_t_data {
    uint32_t head_cluster;
};

struct dir_t_data {
    uint32_t head_cluster;
};

static int read_sector(FATFileSystem* self, uint32_t sector)
{
    if (self->sector_buf_lba != sector || !self->sector_buf_valid) {
        self->sector_buf_lba = sector;
        self->sector_buf_valid = 1;
        return self->bdev_trait->read(
            self->bdev,
            self->sector_buf,
            self->bytes_per_sector,
            sector * self->bytes_per_sector);
    } else {
        return 0;
    }
}

static int read_fat(FATFileSystem* self, void* buf, uint32_t sector, uint32_t count)
{
    return self->bdev_trait->read(
        self->bdev,
        buf,
        count * self->bytes_per_sector,
        (sector + self->lba_offset) * self->bytes_per_sector);
}

static uint16_t get_next_cluster12(FATFileSystem* self, uint16_t current)
{
    uint16_t cluster_pair_idx = (current >> 1) * 3;
    uint8_t* fat = self->fat;
    if (current & 1) {  // odd-numbered cluster
        return ((fat[cluster_pair_idx + 1] & 0xF0) >> 4) | (fat[cluster_pair_idx + 2] << 4);
    } else {  // even-numbered cluster
        return fat[cluster_pair_idx] + ((fat[cluster_pair_idx + 1] & 0x0F) << 8);
    }
}

static uint16_t get_next_cluster16(FATFileSystem* self, uint16_t current)
{
    return byteswap16(((uint16_t*)self->fat)[current]);
}

static uint32_t get_next_cluster32(FATFileSystem* self, uint32_t current)
{
    return byteswap32(((uint32_t*)self->fat)[current]);
}

static uint16_t get_nth_cluster12(FATFileSystem* self, uint16_t head, uint16_t num)
{
    for (; num > 0; num--) {
        head = get_next_cluster12(self, head);
    }
    return head;
}

static uint16_t get_nth_cluster16(FATFileSystem* self, uint16_t head, uint16_t num)
{
    for (; num > 0; num--) {
        head = get_next_cluster16(self, head);
    }
    return head;
}

static uint32_t get_nth_cluster32(FATFileSystem* self, uint32_t head, uint32_t num)
{
    for (; num > 0; num--) {
        head = get_next_cluster32(self, head);
    }
    return head;
}

static uint16_t sector_to_cluster12(FATFileSystem* self, uint16_t sector)
{
    return (sector - self->data_area_begin) / self->data_area_begin;
}

static uint16_t sector_to_cluster16(FATFileSystem* self, uint16_t sector)
{
    return (sector - self->data_area_begin) / self->data_area_begin;
}

static uint32_t sector_to_cluster32(FATFileSystem* self, uint32_t sector)
{
    return (sector - self->data_area_begin) / self->data_area_begin;
}

static uint16_t cluster_to_sector12(FATFileSystem* self, uint16_t cluster)
{
    return (cluster * self->sectors_per_cluster) + self->data_area_begin + self->root_sector_count;
}

static uint16_t cluster_to_sector16(FATFileSystem* self, uint16_t cluster)
{
    return (cluster * self->sectors_per_cluster) + self->data_area_begin + self->root_sector_count;
}

static uint32_t cluster_to_sector32(FATFileSystem* self, uint32_t cluster)
{
    return (cluster * self->sectors_per_cluster) + self->data_area_begin;
}

static uint16_t get_next_sector12(FATFileSystem* self, uint16_t current)
{
    current -= self->data_area_begin;

    if (current % self->sectors_per_cluster != self->sectors_per_cluster - 1) {
        return self->data_area_begin + current + 1;
    }

    current /= self->sectors_per_cluster;
    uint16_t next_cluster = get_next_cluster12(self, current);

    return cluster_to_sector12(self, next_cluster);
}

static uint16_t get_next_sector16(FATFileSystem* self, uint16_t current)
{
    current -= self->data_area_begin;

    if (current % self->sectors_per_cluster != self->sectors_per_cluster - 1) {
        return self->data_area_begin + current + 1;
    }

    current /= self->sectors_per_cluster;
    uint16_t next_cluster = get_next_cluster16(self, current);

    return cluster_to_sector16(self, next_cluster);
}

static uint32_t get_next_sector32(FATFileSystem* self, uint32_t current)
{
    current -= self->data_area_begin;

    if (current % self->sectors_per_cluster != self->sectors_per_cluster - 1) {
        return self->data_area_begin + current + 1;
    }

    current /= self->sectors_per_cluster;
    uint32_t next_cluster = get_next_cluster32(self, current);

    return cluster_to_sector32(self, next_cluster);
}

static uint16_t get_nth_sector12(FATFileSystem* self, uint16_t head_cluster, uint16_t num)
{
    uint8_t cluster_sector_idx = num % self->sectors_per_cluster;
    num /= self->sectors_per_cluster;
    return cluster_to_sector12(self, num) + cluster_sector_idx;
}

static uint16_t get_nth_sector16(FATFileSystem* self, uint16_t head_cluster, uint16_t num)
{
    uint8_t cluster_sector_idx = num % self->sectors_per_cluster;
    num /= self->sectors_per_cluster;
    return cluster_to_sector16(self, num) + cluster_sector_idx;
}

static uint32_t get_nth_sector32(FATFileSystem* self, uint32_t head_cluster, uint32_t num)
{
    uint8_t cluster_sector_idx = num % self->sectors_per_cluster;
    num /= self->sectors_per_cluster;
    return cluster_to_sector32(self, num) + cluster_sector_idx;
}

static int read_next_sector12(FATFileSystem* self, uint16_t current)
{
    current = get_next_cluster12(self, current);
    return read_sector(self, current);
}

static int read_next_sector16(FATFileSystem* self, uint16_t current)
{
    current = get_next_sector16(self, current);
    return read_sector(self, current);
}

static int read_next_sector32(FATFileSystem* self, uint32_t current)
{
    current = get_next_sector32(self, current);
    return read_sector(self, current);
}

static size32_t get_lfn_filename(const union fat_dir_entry* entry, char buf[static FAT_LFN_BUFLEN])
{
    /* support UCS-2 Unicode */
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

static size32_t get_sfn_filename(const union fat_dir_entry* entry, char buf[static FAT_SFN_BUFLEN])
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

static uint8_t get_sfn_checksum(char buf[static FAT_SFN_BUFLEN])
{
    uint8_t chksum = 0;
    for (int i = FAT_SFN_BUFLEN; i != 0; i--) {
        chksum = ((chksum & 1) ? 0x80 : 0) + (chksum >> 1) + *buf++;
    }

    return chksum;
}

int methodof(FATFileSystem, detect)(void* bdev, const DeviceTrait* bdev_trait, uint32_t lba)
{
    struct fat1216_bpb* bpb = mem_alloc(512);
    bdev_trait->read(bdev, bpb, 512, lba * 512);
    int result = bpb->signature == 0x55AA;
    mem_free(bpb);
    return result;
}

int methodof(FATFileSystem, construct)(void* _self, void* bdev, const DeviceTrait* bdev_trait, uint32_t lba)
{
    FATFileSystem* self = _self;

    self->initialized = 0;
    self->sector_buf_valid = 0;
    self->fat_type = FAT_TYPE_UNKNOWN;

    self->bdev = bdev;
    self->bdev_trait = bdev_trait;

    self->sector_buf = mem_alloc(512);
    self->bytes_per_sector = 512;

    // Read sector 0 (BPB)
    read_sector(self, 0);

    const struct fat_common_bpb* bpb = self->sector_buf;
    const struct fat1216_bpb* bpb1216 = self->sector_buf;
    const struct fat32_bpb* bpb32 = self->sector_buf;

    self->sectors_per_cluster = bpb->sectors_per_cluster;
    self->bytes_per_sector = byteswap16(bpb->bytes_per_sector);
    self->root_entry_count = byteswap16(bpb->root_entry_count);
    self->reserved_sector_count = byteswap16(bpb->reserved_sector_count);
    self->root_sector_count = ((self->root_entry_count * 32) + (self->bytes_per_sector - 1)) / self->bytes_per_sector;

    // Determine FAT type
    if (bpb->fat_size16 != 0) {
        self->fat_size = byteswap16(bpb->fat_size16);
    } else {
        self->fat_size = byteswap32(bpb32->fat_size32);
    }

    if (bpb->total_sector_count16 != 0) {
        self->total_sector_count = byteswap16(bpb->total_sector_count16);
    } else {
        self->total_sector_count = byteswap32(bpb->total_sector_count32);
    }

    self->data_area_begin = self->reserved_sector_count + (bpb->fat_count * self->fat_size);
    uint32_t data_sectors = self->total_sector_count - (self->data_area_begin + self->root_sector_count);
    uint32_t cluster_count = data_sectors / self->sectors_per_cluster;

    if (cluster_count < 4085) {
        self->fat_type = FAT_TYPE_FAT12;
    } else if (cluster_count < 65525) {
        self->fat_type = FAT_TYPE_FAT16;
    } else {
        self->fat_type = FAT_TYPE_FAT32;
    }

    // Read FAT
    self->fat = mem_alloc(self->fat_size * self->bytes_per_sector);
    read_fat(self, self->fat, self->reserved_sector_count, 1);  // Reads only 1 sector; fix it later.

    self->initialized = 1;

    if (self->fat_type == FAT_TYPE_FAT32) {
        self->root_cluster = bpb32->root_cluster;

        // Read FSINFO
        read_sector(self, 1);
        const struct fat_fsinfo* fsinfo = self->sector_buf;

        self->free_clusters = fsinfo->free_clusters;
        self->next_free_cluster = fsinfo->next_free_cluster;
    }

    return 0;
}

int methodof(FATFileSystem, destruct)(void* _self)
{
    FATFileSystem* self = _self;

    if (self->initialized) {
        mem_free(self->fat);
        mem_free(self->sector_buf);
    }

    return 0;
}

const char* methodof(FATFileSystem, get_filesystem_name)(void* _self)
{
    FATFileSystem* self = _self;

    static const char* const fat_name[] = {
        "UNKNOWN",
        "FAT12",
        "FAT16",
        "FAT32",
    };

    return fat_name[self->fat_type];
}

int methodof(FATFileSystem, create_directory)(void* _self, const char* path)
{
    return -1;
}

int methodof(FATFileSystem, remove_directory)(void* _self, const char* path)
{
    return -1;
}

int methodof(FATFileSystem, open_directory)(void* _self, dir_t* path, dir_t* dir, const char* name)
{
    FATFileSystem* self = _self;

    if (path == NULL) {
        if (strncmp(name, FS_ROOT_DIR, 3) == 0) {  // use strncmp
            dir->fs = self;
            dir->fstrait = &ftableof(FATFileSystem).impl(FileSystemTrait);
            dir->cursor = 0;
            dir->data = mem_alloc(sizeof(struct dir_t_data));

            if (self->fat_type == FAT_TYPE_FAT32) {
                dir_data(dir)->head_cluster = self->root_cluster;
            } else {
                dir_data(dir)->head_cluster = 0;
            }

            return 0;
        }
    } else {
        const fileinfo_t* fip;
        methodof(FATFileSystem, match_name)(self, path, name, &fip);
        if (fip != NULL) {
            dir->fs = self;
            dir->fstrait = &ftableof(FATFileSystem).impl(FileSystemTrait);
            dir->cursor = 0;
            dir->data = mem_alloc(sizeof(struct dir_t_data));

            const uint16_t cluster_lo = fileinfo_data(fip)->file.cluster_location;
            const uint16_t cluster_hi = fileinfo_data(fip)->file.cluster_location_high;

            union fat_dir_entry* entry = fileinfo_data(fip);
            const uint32_t head_cluster = (byteswap16(cluster_hi) << 16) | byteswap16(cluster_lo);

            dir_data(dir)->head_cluster = head_cluster;
            printf("%s, %08lX\r\n", fileinfo_data(fip)->file.name, head_cluster);
            return 0;
        } else {
            return -1;
        }
    }

    return -1;
}

int methodof(FATFileSystem, close_directory)(void* _self, dir_t* dir)
{
    mem_free(dir->data);
    return 0;
}

int methodof(FATFileSystem, list_directory)(void* _self, dir_t* dir, const fileinfo_t** finfo)
{
    FATFileSystem* self = _self;

    if (!self->initialized) {
        return -1;
    }

    union fat_dir_entry* entries = self->sector_buf;

    int is_lfn = 0;
    uint32_t current_sector_seq;
    uint32_t current_sector_lba;

    if (*finfo == NULL) {
        dir->cursor = 0;
    }

    const uint8_t entries_per_sector = self->bytes_per_sector / sizeof(union fat_dir_entry);

    while (1) {
        current_sector_seq = dir->cursor / entries_per_sector;
        switch (self->fat_type) {
            case FAT_TYPE_FAT12:
                if (is_root(dir)) {
                    current_sector_lba = self->data_area_begin + current_sector_seq;
                } else {
                    current_sector_lba = get_nth_sector12(self, dir_data(dir)->head_cluster, current_sector_seq);
                    printf("LBA 0x%03lX", current_sector_lba);
                }
                break;
            case FAT_TYPE_FAT16:
                if (is_root(dir)) {
                    current_sector_lba = self->data_area_begin + current_sector_seq;
                } else {
                    current_sector_lba = get_nth_sector16(self, dir_data(dir)->head_cluster, current_sector_seq);
                }
                break;
            case FAT_TYPE_FAT32:
                current_sector_lba = get_nth_sector32(self, dir_data(dir)->head_cluster, current_sector_seq);
                break;
            default:
                return -1;
        }
        read_sector(self, current_sector_lba);

        do {
            uint8_t entry_cursor = dir->cursor % entries_per_sector;
            if (!entries[entry_cursor].file.name[0]) {  // End of entry list
                *finfo = NULL;
                return 0;
            } else if (is_lfnentry(&entries[entry_cursor])) {
                get_lfn_filename(&entries[entry_cursor], self->last_entry_lfn);
                is_lfn = 1;
            } else {
                get_sfn_filename(&entries[entry_cursor], self->last_entry_sfn);
                if (is_lfn) {
                    self->last_entry_info.name = self->last_entry_lfn;
                } else {
                    self->last_entry_info.name = self->last_entry_sfn;
                }
                *finfo = &self->last_entry_info;
                self->last_entry_info.data = &(entries[entry_cursor]);
                dir->cursor++;
                return 0;
            }
        } while ((++dir->cursor % entries_per_sector) != 0);
    }
}

int methodof(FATFileSystem, get_created_time)(const fileinfo_t* finfo, timestamp_t* timestamp)
{
    return -1;
}

int methodof(FATFileSystem, get_accessed_time)(const fileinfo_t* finfo, timestamp_t* timestamp)
{
    return -1;
}

int methodof(FATFileSystem, get_modified_time)(const fileinfo_t* finfo, timestamp_t* timestamp)
{
    return -1;
}

int methodof(FATFileSystem, create_file)(void* _self, dir_t* path, const char* name)
{
    return -1;
}

int methodof(FATFileSystem, remove_file)(void* _self, dir_t* path, const char* name)
{
    return -1;
}

int methodof(FATFileSystem, match_name)(void* _self, dir_t* path, const char* name, const fileinfo_t** finfo)
{
    FATFileSystem* self = _self;
    *finfo = NULL;
    do {
        methodof(FATFileSystem, list_directory)(_self, path, finfo);
        if (*finfo != NULL) {
            if (strncmp(name, self->last_entry_sfn, FAT_SFN_BUFLEN) == 0) {
                return 0;
            } else if (strncmp(name, self->last_entry_lfn, FAT_LFN_BUFLEN) == 0) {
                return 0;
            }
        }
    } while (*finfo != NULL);
    *finfo = NULL;
    return 0;
}

int methodof(FATFileSystem, open_file)(void* _self, dir_t* path, file_t* file, const char* name, const char* mode)
{
    return -1;
}

int methodof(FATFileSystem, close_file)(void* _self, file_t* file)
{
    return -1;
}

int methodof(FATFileSystem, read_file)(void* _self, file_t* file)
{
    return -1;
}

int methodof(FATFileSystem, write_file)(void* _self, file_t* file)
{
    return -1;
}

int methodof(FATFileSystem, seek_file)(void* _self, file_t* file)
{
    return -1;
}

int methodof(FATFileSystem, test_file)(void* _self, dir_t* dir, const char* name)
{
    return -1;
}

int methodof(FATFileSystem, move)(void* _self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname)
{
    return -1;
}

int methodof(FATFileSystem, copy)(void* _self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname)
{
    return -1;
}

