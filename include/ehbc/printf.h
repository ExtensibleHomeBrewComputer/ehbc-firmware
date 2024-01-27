#ifndef _EHBC_PRINTF_H__
#define _EHBC_PRINTF_H__

#include <stdarg.h>

#include <ehbc/types.h>
#include <ehbc/iodev.h>
#include <ehbc/units.h>
#include <ehbc/class.h>
#include <ehbc/hw/device.h>

__attribute__((format(printf, 3, 0)))
int vsnprintf(char* buf, size32_t len, const char* fmt, va_list ap);

__attribute__((format(printf, 3, 4)))
static inline int snprintf(char* buf, size32_t len, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vsnprintf(buf, len, fmt, ap);
    va_end(ap);
    return result;
}

__attribute__((format(printf, 1, 2)))
static inline int printf(const char* fmt, ...)
{
    static char printf_buf[1 * kiB];
    va_list ap;
    va_start(ap, fmt);
    int bytes = vsnprintf(printf_buf, sizeof(printf_buf), fmt, ap);
    va_end(ap);
    class_t* odev = get_io_device();
    return methodof(Device, write)(odev, printf_buf, bytes, 0);
}

#endif  // _EHBC_PRINTF_H__
