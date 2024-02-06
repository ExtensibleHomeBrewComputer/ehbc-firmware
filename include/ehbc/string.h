#ifndef _EHBC_STRING_H__
#define _EHBC_STRING_H__

#include <ehbc/types.h>
#include <ehbc/ctype.h>
#include <ehbc/printf.h>

int strncmp(const char* str1, const char* str2, size32_t count);
int strnicmp(const char* str1, const char* str2, size32_t count);
char* strncpy(char* dest, const char* src, size32_t count);
size32_t strnlen(const char* str, size32_t count);

void* memcpy(void* dest, const void* src, size32_t count);
void* memset(void* dest, int c, size_t count);

#endif  // _EHBC_STRING_H__
