#ifndef _EHBC_FS_FAT_H__
#define _EHBC_FS_FAT_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/fs/filesystem.h>

#define FAT_SFN_NAME        8
#define FAT_SFN_EXTENSION   3
#define FAT_SFN_LENGTH      (FAT_SFN_NAME + FAT_SFN_EXTENSION)
#define FAT_LFN_LENGTH      255
#define FAT_SFN_BUFLEN      FAT_SFN_LENGTH + 2  // "filename" + '.' + "ext" + '\0'
#define FAT_LFN_BUFLEN      FAT_LFN_LENGTH + 1  // "longfilename" + '\0'


typedef struct {
    void* bdev;
    const DeviceTrait* bdev_trait;

    uint32_t lba_offset;

    void*    sector_buf;
    uint32_t sector_buf_lba;

    void*    fat;

    char       last_entry_lfn[FAT_LFN_BUFLEN];
    char       last_entry_sfn[FAT_SFN_BUFLEN];
    fileinfo_t last_entry_info;

    uint16_t reserved_sector_count;
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint8_t  initialized : 1;
    uint8_t  sector_buf_valid : 1;
    uint8_t  fat_type    : 2;
    uint32_t data_area_begin;
    uint32_t fat_size;
    uint32_t free_clusters;
    uint32_t next_free_cluster;
    uint32_t total_sector_count;
    uint32_t root_cluster;
    uint16_t root_entry_count;
    uint16_t root_sector_count;
} FATFileSystem;

int memberof(FATFileSystem, detect)(void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);

int memberof(FATFileSystem, construct)(void* self, void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);
int memberof(FATFileSystem, destruct)(void* self);

const char* memberof(FATFileSystem, get_filesystem_name)(void* self);

int memberof(FATFileSystem, match_name)(void* self, dir_t* path, const char* name, const fileinfo_t** finfo);

int memberof(FATFileSystem, create_directory)(void* self, const char* path);
int memberof(FATFileSystem, remove_directory)(void* self, const char* path);
int memberof(FATFileSystem, open_directory)(void* self, dir_t* path, dir_t* dir, const char* name);
int memberof(FATFileSystem, close_directory)(void* self, dir_t* dir);
int memberof(FATFileSystem, list_directory)(void* self, dir_t* dir, const fileinfo_t** finfo);

int memberof(FATFileSystem, get_created_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
int memberof(FATFileSystem, get_modified_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
int memberof(FATFileSystem, get_accessed_time)(const fileinfo_t* finfo, timestamp_t* timestamp);

int memberof(FATFileSystem, create_file)(void* self, dir_t* path, const char* name);
int memberof(FATFileSystem, remove_file)(void* self, dir_t* path, const char* name);
int memberof(FATFileSystem, open_file)(void* self, dir_t* path, file_t* file, const char* name, const char* mode);
int memberof(FATFileSystem, close_file)(void* self, file_t* file);
size32_t memberof(FATFileSystem, read_file)(void* self, file_t* file, void* buf, size32_t size, size32_t count);
size32_t memberof(FATFileSystem, write_file)(void* self, file_t* file, const void* buf, size32_t size, size32_t count);
int memberof(FATFileSystem, seek_file)(void* self, file_t* file, ssize32_t offset, int origin);
ssize32_t memberof(FATFileSystem, tell_file)(void* self, file_t* file);
int memberof(FATFileSystem, is_eof)(void* self, file_t* file);
int memberof(FATFileSystem, test_file)(void* self, dir_t* dir, const char* name);

int memberof(FATFileSystem, move)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);
int memberof(FATFileSystem, copy)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);

static const struct {
    FileSystemTrait impl(FileSystemTrait);
} ftableof(FATFileSystem) = {
    .impl(FileSystemTrait) = {
        .detect = memberof(FATFileSystem, detect),

        .construct = memberof(FATFileSystem, construct),
        .destruct = memberof(FATFileSystem, destruct),

        .get_filesystem_name = memberof(FATFileSystem, get_filesystem_name),

        .create_directory = memberof(FATFileSystem, create_directory),
        .remove_directory = memberof(FATFileSystem, remove_directory),
        .open_directory = memberof(FATFileSystem, open_directory),
        .close_directory = memberof(FATFileSystem, close_directory),
        .list_directory = memberof(FATFileSystem, list_directory),

        .get_created_time = memberof(FATFileSystem, get_created_time),
        .get_modified_time = memberof(FATFileSystem, get_modified_time),
        .get_accessed_time = memberof(FATFileSystem, get_accessed_time),

        .create_file = memberof(FATFileSystem, create_file),
        .remove_file = memberof(FATFileSystem, remove_file),
        .open_file = memberof(FATFileSystem, open_file),
        .close_file = memberof(FATFileSystem, close_file),
        .read_file = memberof(FATFileSystem, read_file),
        .write_file = memberof(FATFileSystem, write_file),
        .seek_file = memberof(FATFileSystem, seek_file),
        .is_eof = memberof(FATFileSystem, is_eof),
        .test_file = memberof(FATFileSystem, test_file),

        .move = memberof(FATFileSystem, move),
        .copy = memberof(FATFileSystem, copy),
    }
};

#endif  // _EHBC_FS_FAT_H__
