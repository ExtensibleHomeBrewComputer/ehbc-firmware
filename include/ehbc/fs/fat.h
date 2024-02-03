#ifndef _EHBC_FS_FAT_H__
#define _EHBC_FS_FAT_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/fs/filesystem.h>

#define FAT_SFN_NAME        8
#define FAT_SFN_EXTENSION   3
#define FAT_SFN_LENGTH      (FAT_SFN_NAME + FAT_SFN_EXTENSION)
#define FAT_LFN_LENGTH      255
#define FAT_SFN_BUFLEN      FAT_SFN_LENGTH + 2  // "filename" + "." + "ext" + "\0"
#define FAT_LFN_BUFLEN      FAT_LFN_LENGTH + 1  // "longfilename" + "\0"


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

int methodof(FATFileSystem, detect)(void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);

int methodof(FATFileSystem, construct)(void* self, void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);
int methodof(FATFileSystem, destruct)(void* self);

const char* methodof(FATFileSystem, get_filesystem_name)(void* self);

int methodof(FATFileSystem, match_name)(void* self, dir_t* path, const char* name, const fileinfo_t** finfo);

int methodof(FATFileSystem, create_directory)(void* self, const char* path);
int methodof(FATFileSystem, remove_directory)(void* self, const char* path);
int methodof(FATFileSystem, open_directory)(void* self, dir_t* path, dir_t* dir, const char* name);
int methodof(FATFileSystem, close_directory)(void* self, dir_t* dir);
int methodof(FATFileSystem, list_directory)(void* self, dir_t* dir, const fileinfo_t** finfo);

int methodof(FATFileSystem, get_created_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
int methodof(FATFileSystem, get_modified_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
int methodof(FATFileSystem, get_accessed_time)(const fileinfo_t* finfo, timestamp_t* timestamp);

int methodof(FATFileSystem, create_file)(void* self, dir_t* path, const char* name);
int methodof(FATFileSystem, remove_file)(void* self, dir_t* path, const char* name);
int methodof(FATFileSystem, open_file)(void* self, dir_t* path, file_t* file, const char* name, const char* mode);
int methodof(FATFileSystem, close_file)(void* self, file_t* file);
int methodof(FATFileSystem, read_file)(void* self, file_t* file);
int methodof(FATFileSystem, write_file)(void* self, file_t* file);
int methodof(FATFileSystem, seek_file)(void* self, file_t* file);
int methodof(FATFileSystem, test_file)(void* self, dir_t* dir, const char* name);

int methodof(FATFileSystem, move)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);
int methodof(FATFileSystem, copy)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);

static const struct {
    FileSystemTrait impl(FileSystemTrait);
} ftableof(FATFileSystem) = {
    .impl(FileSystemTrait) = {
        .detect = methodof(FATFileSystem, detect),

        .construct = methodof(FATFileSystem, construct),
        .destruct = methodof(FATFileSystem, destruct),

        .get_filesystem_name = methodof(FATFileSystem, get_filesystem_name),

        .create_directory = methodof(FATFileSystem, create_directory),
        .remove_directory = methodof(FATFileSystem, remove_directory),
        .open_directory = methodof(FATFileSystem, open_directory),
        .close_directory = methodof(FATFileSystem, close_directory),
        .list_directory = methodof(FATFileSystem, list_directory),

        .get_created_time = methodof(FATFileSystem, get_created_time),
        .get_modified_time = methodof(FATFileSystem, get_modified_time),
        .get_accessed_time = methodof(FATFileSystem, get_accessed_time),

        .create_file = methodof(FATFileSystem, create_file),
        .remove_file = methodof(FATFileSystem, remove_file),
        .open_file = methodof(FATFileSystem, open_file),
        .close_file = methodof(FATFileSystem, close_file),
        .read_file = methodof(FATFileSystem, read_file),
        .write_file = methodof(FATFileSystem, write_file),
        .seek_file = methodof(FATFileSystem, seek_file),
        .test_file = methodof(FATFileSystem, test_file),

        .move = methodof(FATFileSystem, move),
        .copy = methodof(FATFileSystem, copy),
    }
};

#endif  // _EHBC_FS_FAT_H__
