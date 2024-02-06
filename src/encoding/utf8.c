#include <ehbc/encoding/utf8.h>

#include <ehbc/string.h>

size32_t memberof(UTF8, decode)(const void* encoded, uraw_t* decoded, size32_t size)
{
    const UTF8* seq = encoded;
    if (!(*seq & 0x80)) {
        *decoded = *seq;
        return 1;
    } else if (*seq >> 5 == 0xC0) {
        *decoded = ((seq[0] & 0x1F) << 6) | (seq[1] & 0x3F);
        return 2;
    } else {
        *decoded = ((seq[0] & 0x0F) << 12) | ((seq[1] & 0x3F) << 6) | (seq[2] & 0x3F);
        return 3;
    }
}

size32_t memberof(UTF8, encode)(uraw_t decoded, void* encoded, size32_t size)
{
    UTF8* seq = encoded;

    if (decoded < 0x7F) {
        if (size < 1) {
            return 0;
        }
        *seq = decoded;
        return 1;
    } else if (decoded < 0x7FF) {
        if (size < 2) {
            return 0;
        }
        *seq++ = ((decoded & 0x07C0) >> 6) | 0xC0;
        *seq++ = (decoded & 0x003F) | 0x80;
        return 2;
    } else {
        if (size < 3) {
            return 0;
        }
        *seq++ = ((decoded & 0xF000) >> 12) | 0xE0;
        *seq++ = ((decoded & 0x0FC0) >> 6) | 0x80;
        *seq++ = (decoded & 0x003F) | 0x80;
        return 3;
    }
}

size32_t memberof(UTF8, chrbnlen)(const void* seq, size32_t size)
{
    if (*(const uint8_t*)seq < 0x7F) {
        return size < 1 ? 0 : 1;
    } else if (*(const uint8_t*)seq < 0x7FF) {
        return size < 2 ? 0 : 2;
    } else {
        return size < 3 ? 0 : 3;
    }
}

size32_t memberof(UTF8, strcnlen)(const void* str, size32_t size)
{
    const uint8_t* bstr = str;
    size32_t len = 0;
    while (size > 0) {
        size32_t chrlen = memberof(UTF8, chrbnlen)(bstr, size);
        if (chrlen == 0) {
            break;
        }
        size -= chrlen;
        bstr += chrlen;
        len++;
    }
    return len;
}

size32_t  memberof(UTF8, strbnlen)(const void* str, size32_t size)
{
    return strnlen(str, size);
}

/*
int u8strncmp(const UTF8* str1, const UTF8* str2, size32_t count)
{
    for (; count > 0; count--) {
        if (*str1 == 0 && *str2 == 0) {
            return 0;
        }

        uraw_t ch1, ch2;
        str1 = decode_u8(str1, &ch1);
        str2 = decode_u8(str2, &ch2);

        if (ch1 != ch2) {
            return ch1 - ch2;
        }
    }
    return 0;
}

void hexdump(const void* p, size_t len);

int u8strnicmp(const UTF8* str1, const UTF8* str2, size32_t count)
{
    // TODO: support for non-ascii characters
    for (; count > 0; count--) {
        if (*str1 == 0 && *str2 == 0) {
            return 0;
        }

        uraw_t ch1, ch2;
        str1 = decode_u8(str1, &ch1);
        str2 = decode_u8(str2, &ch2);

        if (ch1 != ch2 && (toupper(ch1) != toupper(ch2) || ch1 >= 0x80 || ch2 >= 0x80)) {
            return ch1 - ch2;
        }
    }
    return 0;
}


size32_t convert_ucs2_u8(UTF8* dest, const ucs2char_t* src, size32_t dest_size)
{
    size32_t written_bytes = 0;

    while (*src != 0) {
        UTF8 temp_str[3];
        int temp_len = 0;

        if (*src < 0x7F) {
            temp_str[temp_len++] = *src;
        } else if (*src < 0x7FF) {
            temp_str[temp_len++] = ((*src & 0x07C0) >> 6) | 0xC0;
            temp_str[temp_len++] = (*src & 0x003F) | 0x80;
        } else {
            temp_str[temp_len++] = ((*src & 0xF000) >> 12) | 0xE0;
            temp_str[temp_len++] = ((*src & 0x0FC0) >> 6) | 0x80;
            temp_str[temp_len++] = (*src & 0x003F) | 0x80;
        }

        if (written_bytes + temp_len > dest_size) {
            break;
        } else {
            for (int i = 0; i < temp_len; i++) {
                *dest++ = temp_str[i];
                written_bytes++;
            }
        }

        src++;
    }

    if (written_bytes < dest_size) {
        *dest = 0;
        written_bytes++;
    }

    return written_bytes;
}
*/
