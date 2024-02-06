#ifndef _EHBC_FS_FILESYSTEM_H__
#define _EHBC_FS_FILESYSTEM_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>
#include <ehbc/hw/device.h>
#include <ehbc/encoding/encoding.h>
#include <ehbc/encoding/utf8.h>

#define FS_ROOT_DIR utf8("::")

#define FS_SEEK_SET 0
#define FS_SEEK_CUR 1
#define FS_SEEK_END 2

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

    int (*match_name)(void* self, dir_t* path, const UTF8* name, const fileinfo_t** finfo);

    int (*create_directory)(void* self, dir_t* path, const UTF8* name);
    int (*remove_directory)(void* self, dir_t* path, const UTF8* name);
    int (*open_directory)(void* self, dir_t* path, dir_t* dir, const UTF8* name);
    int (*close_directory)(void* self, dir_t* dir);
    int (*list_directory)(void* self, dir_t* dir, const fileinfo_t** finfo);

    int (*get_created_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
    int (*get_modified_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
    int (*get_accessed_time)(const fileinfo_t* finfo, timestamp_t* timestamp);
    ssize32_t (*get_file_size)(const fileinfo_t* finfo);

    int (*create_file)(void* self, dir_t* path, const UTF8* name);
    int (*remove_file)(void* self, dir_t* path, const UTF8* name);
    int (*open_file)(void* self, dir_t* path, file_t* file, const UTF8* name, const char* mode);
    int (*close_file)(void* self, file_t* file);
    size32_t (*read_file)(void* self, file_t* file, void* buf, size32_t size, size32_t count);
    size32_t (*write_file)(void* self, file_t* file, const void* buf, size32_t size, size32_t count);
    int (*seek_file)(void* self, file_t* file, ssize32_t offset, int origin);
    ssize32_t (*tell_file)(void* self, file_t* file);
    int (*is_eof)(void* self, file_t* file);
    int (*test_file)(void* self, dir_t* dir, const UTF8* name);

    int (*move)(void* self, dir_t* origdir, const UTF8* origname, dir_t* tgtdir, const UTF8* tgtname);
    int (*copy)(void* self, dir_t* origdir, const UTF8* origname, dir_t* tgtdir, const UTF8* tgtname);
} FileSystemTrait;

struct file {
    void* fs;
    const FileSystemTrait* fstrait;
    ssize32_t cursor;
    int error;
    void* data;
};

struct fileinfo {
    const UTF8* name;
    void* data;
};

#endif  // _EHBC_FS_FILESYSTEM_H__
