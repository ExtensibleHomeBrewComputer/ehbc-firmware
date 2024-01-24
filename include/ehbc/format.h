#ifndef _EHBC_FORMAT_H__
#define _EHBC_FORMAT_H__

#include <stdarg.h>

#include <ehbc/types.h>

__attribute__((format(printf, 3, 0)))
int vformat(char* buf, size32_t len, const char* fmt, va_list ap);

__attribute__((format(printf, 3, 4)))
static inline int format(char* buf, size32_t len, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int result = vformat(buf, len, fmt, ap);
    va_end(ap);
    return result;
}


#endif  // _EHBC_FORMAT_H__
