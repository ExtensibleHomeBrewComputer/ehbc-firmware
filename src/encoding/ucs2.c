#include <ehbc/encoding/ucs2.h>

#include <ehbc/string.h>

size32_t memberof(UCS2, decode)(const void* encoded, uraw_t* decoded, size32_t size)
{
    const UCS2* seq = encoded;
    *decoded = *seq;
    return 2;
}

size32_t memberof(UCS2, encode)(uraw_t decoded, void* encoded, size32_t size)
{
    UCS2* seq = encoded;
    if (size < 1 || decoded > 0xFFFF) {
        return 0;
    } else {
        *seq = (UCS2)decoded;
        return 2;
    }
}

size32_t memberof(UCS2, chrbnlen)(const void* seq, size32_t size)
{
    return 2;
}

size32_t memberof(UCS2, strcnlen)(const void* str, size32_t size)
{
    const UCS2* ucs2str = str;
    size32_t len = 0;
    while (*ucs2str++ != 0 && size-- > 0) {
        len++;
    }
    return len;
}

size32_t  memberof(UCS2, strbnlen)(const void* str, size32_t size)
{
    const UCS2* ucs2str = str;
    size32_t len = 0;
    while (*ucs2str++ != 0 && size-- > 0) {
        len++;
    }
    return len << 1;
}
