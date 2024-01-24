#ifndef _EHBC_FS_PABLE_MBR_H__
#define _EHBC_FS_PABLE_MBR_H__

#include <ehbc/types.h>

#define MBR_SIGNATURE   0xAA55

struct ptable_mbr_entry {
    uint8_t                 boot_flag;
    uint8_t                 chs_location[3];
    uint8_t                 partition_type;
    uint8_t                 chs_end_location[3];
    uint32_t                lba_location;
    uint32_t                lba_size;
};

struct ptable_mbr {
    uint8_t                 x86_boot_code[446];
    struct ptable_mbr_entry partition_entries[4];
    uint16_t                signature;
};

#endif  // _EHBC_FS_PTABLE_MBR_H__
