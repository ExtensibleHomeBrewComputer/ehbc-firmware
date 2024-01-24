#ifndef _EHBC_FS_PARTITION_H__
#define _EHBC_FS_PARTITION_H__

#include <ehbc/types.h>

struct partition {
    struct block_device* blkdev;
    uint32_t lba_offset;
    uint32_t lba_size;
};

struct partition_table {
    struct block_device* blkdev;
    uint32_t lba_offset;
    uint32_t lba_size;
};

#endif  // _EHBC_FS_PARTITION_H__
