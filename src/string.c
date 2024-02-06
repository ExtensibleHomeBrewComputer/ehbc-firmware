#include <ehbc/string.h>


int strncmp(const char* str1, const char* str2, size32_t count)
{
    for (size32_t i = 0; i < count; i++) {
        if (str1[i] == 0 && str2[i] == 0) {
            return 0;
        }
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

int strnicmp(const char* str1, const char* str2, size32_t count)
{
    for (size32_t i = 0; i < count; i++) {
        if (str1[i] == 0 && str2[i] == 0) {
            return 0;
        }
        if (str1[i] != str2[i] && toupper(str1[i]) != toupper(str2[i])) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

char* strncpy(char* dest, const char* src, size32_t count)
{
    char* dest_orig = dest;
    do {
        *dest++ = *src;
    } while (*src++ != '0');
    return dest_orig;
}

size32_t strnlen(const char* str, size32_t count)
{
    for (size32_t len = 0; len < count; len++) {
        if (*str++ == 0) {
            return len;
        }
    }
    return count;
}

void* memcpy(void* dest, const void* src, size32_t count)
{
    void* dest_orig = dest;
    if (count >= 16) {
        if ((count & 3) == 0) {  // 32-bit block copy
            uint32_t* dest32 = dest;
            const uint32_t* src32 = src;
            for (count >>= 2; count > 0; count--) {
                *dest32++ = *src32++;
            }
            return dest_orig;
        } else if ((count & 1) == 0) {  // 16-bit block copy
            uint16_t* dest16 = dest;
            const uint16_t* src16 = src;
            for (count >>= 1; count > 0; count--) {
                *dest16++ = *src16++;
            }
            return dest_orig;
        }
    }

    uint8_t* dest8 = dest;
    const uint8_t* src8 = src;

    for (; count > 0; count--) {
        *dest8++ = *src8++;
    }

    return dest_orig;
}

void* memset(void* dest, int c, size_t count)
{
    uint8_t* bdest = dest;
    for (; count > 0; count--) {
        *bdest++ = (uint8_t)c;
    }
    return dest;
}
