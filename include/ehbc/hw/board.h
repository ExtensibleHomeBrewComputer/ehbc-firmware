#ifndef _EHBC_HW_BOARD_H__
#define _EHBC_HW_BOARD_H__

#include <ehbc/types.h>
#include <ehbc/hw/device.h>

struct board {
    const char* name;
    const char* description;
    const struct device* devices;
};

#endif  // _EHBC_HW_BOARD_H__
