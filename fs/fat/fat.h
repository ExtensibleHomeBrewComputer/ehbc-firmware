#ifndef _EHBC_FS_FAT_H__
#define _EHBC_FS_FAT_H__

#include <ehbc/types.h>

#define FAT_SECTOR_SIZE         512

#define FAT_SFN_NAME            8
#define FAT_SFN_EXTENSION       3
#define FAT_SFN_LENGTH          (FAT_SFN_NAME + FAT_SFN_EXT)
#define FAT_LFN_LENGTH          256


#define FAT12_MAX_CLUSTER       0xFF4
#define FAT16_MAX_CLUSTER       0xFFF4
#define FAT32_MAX_CLUSTER       0x0FFFFFF6

#define FAT12_BAD_CLUSTER       0xFF7
#define FAT16_BAD_CLUSTER       0xFFF7
#define FAT32_BAD_CLUSTER       0x0FFFFFF7

#define FAT12_END_CLUSTER       0xFFF
#define FAT16_END_CLUSTER       0xFFFF
#define FAT32_END_CLUSTER       0x0FFFFFFF

#define FAT_BPB_SIGNATURE       0xAA55

#define FAT_FSINFO_SIGNATURE1   0x41615252
#define FAT_FSINFO_SIGNATURE2   0x61417272
#define FAT_FSINFO_SIGNATURE3   (FAT_BPB_SIGNATURE)

struct fat_bpb {
    uint8_t        x86_jump_code[3];
    uint8_t        oem_name[8];
    uint16_t       bytes_per_sector;
    uint8_t        sectors_per_cluster;
    uint16_t       reserved_sector_count;
    uint8_t        fat_count;
    uint16_t       root_entry_count;
    uint16_t       total_sector_count16;
    uint8_t        media_type;
    uint16_t       fat_size;
    uint16_t       sectors_per_track;
    uint16_t       head_count;
    uint32_t       hidden_sector_count;
    uint32_t       total_sector_count32;

    uint32_t       fat32_size;
    uint16_t       flags;
    uint16_t       version;
    uint32_t       root_cluster;
    uint16_t       fsinfo_sector;
    uint16_t       bpb_backup_sector;
    uint8_t        __reserved1[12];
    uint8_t        physical_drive_num;
    uint8_t        __reserved2;
    uint8_t        extended_boot_signature;
    uint32_t       volume_serial;
    uint8_t        volume_label[11];
    uint8_t        fs_type[8];

    uint8_t        boot_code[420];
    uint16_t       signature;
};

struct fat_fsinfo {
    uint32_t        signature1;
    uint8_t         __reserved1[480];
    uint32_t        signature2;
    uint32_t        free_clusters;
    uint32_t        next_free_cluster;
    uint32_t        __reserved2[14];
    uint16_t        signature3;
};

struct fat_directory_entry {
    uint8_t         name[FAT_SFN_NAME];
    uint8_t         extension[FAT_SFN_EXTENSION];
    uint8_t         attribute;
    uint8_t         __reserved;
    uint8_t         created_tenth;
    uint16_t        created_time;
    uint16_t        created_date;
    uint16_t        accessed_date;
    uint16_t        cluster_location_high;
    uint16_t        modified_time;
    uint16_t        modified_date;
    uint16_t        cluster_location;
    uint32_t        size;
};

struct fat_lfn_entry {
    uint8_t         sequence_index;
    uint16_t        name_fragment1[5];
    uint8_t         attribute;
    uint8_t         __reserved;
    uint8_t         checksum;
    uint16_t        name_fragment2[6];
    uint16_t        cluster_location;
    uint16_t        name_fragment3[2];
};

union fat_dos_time {
    uint16_t raw;
    struct {
        uint16_t second_div2 : 5;
        uint16_t minute : 6;
        uint16_t hour : 5;
    };
};

union fat_dos_date {
    uint16_t raw;
    struct {
        uint16_t day : 5;
        uint16_t month : 4;
        uint16_t year : 7;
    };
};

#endif  // _EHBC_FS_FAT_H__
