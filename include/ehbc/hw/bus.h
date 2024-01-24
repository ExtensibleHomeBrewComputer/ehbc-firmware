#ifndef _EHBC_W_BUS_H__
#define _EHBC_W_BUS_H__

#include <ehbc/types.h>

#define BUS_MMIO NULL

struct bus {
    const char* name;
    const char* description;
};

#endif  // _EHBC_W_BUS_H__
