#include <ehbc/encoding/utf8.h>

#include <ehbc/string.h>
#include <ehbc/printf.h>

uraw_t memberof(Encoding, ctolower)(uraw_t c)
{
    return c < 0x80 ? tolower(c) : c;
}

uraw_t memberof(Encoding, ctoupper)(uraw_t c)
{
    return c < 0x80 ? toupper(c) : c;
}

size32_t memberof(Encoding, convert)(const EncodingTrait* tdest, void* dest, size32_t dsize, const EncodingTrait* tsrc, const void* src, size32_t ssize)
{
    uint8_t* bdest = dest;
    const uint8_t* bsrc = src;

    uraw_t uraw_buf;

    size32_t chrsztmp, bytes_written = 0;
    while (ssize > 0 && dsize > 0) {
        chrsztmp = tsrc->decode(bsrc, &uraw_buf, ssize);
        if (chrsztmp == 0) {
            break;
        }
        bsrc += chrsztmp;
        ssize -= chrsztmp;

        chrsztmp = tdest->encode(uraw_buf, bdest, dsize);
        if (chrsztmp == 0) {
            break;
        }
        bdest += chrsztmp;
        dsize -= chrsztmp;
        bytes_written += chrsztmp;
    }

    return bytes_written;
}

int memberof(Encoding, strncmp)(const EncodingTrait* trait, const void* str1, const void* str2, size32_t size)
{
    uraw_t ch1, ch2;
    size32_t chr_size;
    const uint8_t* bstr1 = str1;
    const uint8_t* bstr2 = str2;
    
    while (size > 0) {
        bstr1 += trait->decode(bstr1, &ch1, size);
        bstr2 += chr_size = trait->decode(bstr2, &ch2, size);
        size -= chr_size;
        if (ch1 == 0 && ch2 == 0) {
            return 0;
        }

        if (ch1 != ch2) {
            return ch1 - ch2;
        }
    }
    return 0;
}

int memberof(Encoding, strnicmp)(const EncodingTrait* trait, const void* str1, const void* str2, size32_t size)
{
    uraw_t ch1, ch2;
    size32_t chr_size;
    const uint8_t* bstr1 = str1;
    const uint8_t* bstr2 = str2;
    
    while (size > 0) {
        bstr1 += trait->decode(bstr1, &ch1, size);
        bstr2 += chr_size = trait->decode(bstr2, &ch2, size);
        size -= chr_size;
        if (ch1 == 0 && ch2 == 0) {
            return 0;
        }

        if (ch1 != ch2 && memberof(Encoding, ctoupper)(ch1) != memberof(Encoding, ctoupper)(ch2)) {
            return ch1 - ch2;
        }
    }
    return 0;
}
