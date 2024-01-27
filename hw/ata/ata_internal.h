#ifndef _EHBC_HW_ATA_ATA_INTERNAL_H__
#define _EHBC_HW_ATA_ATA_INTERNAL_H__

#include <ehbc/types.h>

#define ATA_REG_DATA        0
#define ATA_REG_ERROR       1
#define ATA_REG_FEATURES    1
#define ATA_REG_SECT_COUNT  2
#define ATA_REG_SECT_NUM    3
#define ATA_REG_CYL_LOW     4
#define ATA_REG_CYL_HIGH    5
#define ATA_REG_DRV_HEAD    6
#define ATA_REG_LBA_LOW     3
#define ATA_REG_LBA_MID     4
#define ATA_REG_LBA_HIGH    5
#define ATA_REG_STATUS      7
#define ATA_REG_COMMAND     7

#endif  // _EHBC_HW_ATA_ATA_INTERNAL_H__
