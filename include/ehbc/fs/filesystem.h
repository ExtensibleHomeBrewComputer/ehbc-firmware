#ifndef _EHBC_FS_FILESYSTEM_H__
#define _EHBC_FS_FILESYSTEM_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/hw/device.h>

#define FS_ROOT_DIR "::"

struct file;
struct fileinfo;

typedef struct file file_t;
typedef struct file dir_t;
typedef struct fileinfo fileinfo_t;

typedef struct {
    uint16_t year;
    uint32_t month : 4;
    uint32_t day : 5;
    uint32_t hour : 5;
    uint32_t minute : 6;
    uint32_t second : 6;
    uint32_t : 6;
    uint32_t nsec;
} timestamp_t;

typedef struct {
    int (*detect)(void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);

    int (*construct)(void* self, void* bdev, const DeviceTrait* bdev_trait, uint32_t lba);
    int (*destruct)(void* self);

    const char* (*get_filesystem_name)(void* self);

    int (*match_name)(void* self, dir_t* path, const char* name, const fileinfo_t** finfo);

    int (*create_directory)(void* self, const char* path);
    int (*remove_directory)(void* self, const char* path);
    int (*open_directory)(void* self, dir_t* path, dir_t* dir, const char* name);
    int (*close_directory)(void* self, dir_t* dir);
    int (*list_directory)(void* self, dir_t* dir, const fileinfo_t** finfo);

    int (*get_created_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
    int (*get_modified_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
    int (*get_accessed_time)(const fileinfo_t* finfo, timestamp_t* timestamp);

    int (*create_file)(void* self, dir_t* path, const char* name);
    int (*remove_file)(void* self, dir_t* path, const char* name);
    int (*open_file)(void* self, dir_t* path, file_t* file, const char* name, const char* mode);
    int (*close_file)(void* self, file_t* file);
    int (*read_file)(void* self, file_t* file);
    int (*write_file)(void* self, file_t* file);
    int (*seek_file)(void* self, file_t* file);
    int (*test_file)(void* self, dir_t* dir, const char* name);

    int (*move)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);
    int (*copy)(void* self, dir_t* origdir, const char* origname, dir_t* tgtdir, const char* tgtname);
} FileSystemTrait;

struct file {
    void* fs;
    const FileSystemTrait* fstrait;
    size32_t cursor;
    void* data;
};

struct fileinfo {
    const char* name;
    void* data;
};

#endif  // _EHBC_FS_FILESYSTEM_H__
