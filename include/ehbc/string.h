#ifndef _EHBC_STRING_H__
#define _EHBC_STRING_H__

#include <ehbc/types.h>

static inline int strncmp(const char* str1, const char* str2, size32_t num)
{
    for (size32_t i = 0; i < num; i++) {
        if (str1[i] == 0 && str2[i] == 0) {
            return 0;
        }
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

#endif  // _EHBC_STRING_H__
