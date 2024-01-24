#ifndef _EHBC_FS_FILESYSTEM_H__
#define _EHBC_FS_FILESYSTEM_H__

#include <ehbc/types.h>
#include "fs/file.h"
#include "fs/ptable.h"

struct fs_operations;

struct fs_mount_info {
    struct partition_table* ptable;
    struct fs_operations*   operations;

    uint32_t        mount_id;

    const char*     name;
    uint32_t        offset_lba;
    uint32_t        size_lba;

    uint16_t        is_initialized : 1;
    uint16_t        is_mounted : 1;
    uint16_t        : 14;
};

struct fs_operations {
    /// const fs_mount_info* minfo
    int (*mount)(const struct fs_mount_info*);
    /// 
    int (*unmount)(const struct fs_mount_info*);
    /// const char* path, const char* mode, struct fs_file* file
    int (*open)(const char*, const char*, struct fs_file*);
    /// struct fs_file* file
    int (*close)(struct fs_file*);
    ///
    int (*read)(struct fs_file*, uint8_t*, size32_t, uint32_t);
    ///
    int (*write)(struct fs_file*, const uint8_t*, size32_t, uint32_t);
    int (*flush)()
};


#endif  // _EHBC_FS_FILESYSTEM_H__
