#ifndef _EHBC_ENCODING_UCS2_H__
#define _EHBC_ENCODING_UCS2_H__

#include <ehbc/types.h>
#include <ehbc/encoding/encoding.h>
#include <ehbc/struct.h>
#include <ehbc/string.h>

size32_t memberof(ASCII, decode)(const void* encoded, uraw_t* decoded, size32_t size);
size32_t memberof(ASCII, encode)(uraw_t decoded, void* encoded, size32_t size);

size32_t memberof(ASCII, chrbnlen)(const void* seq, size32_t size);
size32_t memberof(ASCII, strcnlen)(const void* str, size32_t size);
size32_t memberof(ASCII, strbnlen)(const void* str, size32_t size);

static const EncodingTrait impl(ASCII, EncodingTrait) = {
    .decode = memberof(ASCII, decode),
    .encode = memberof(ASCII, encode),
    .chrbnlen = (size32_t (*)(const void*, size32_t))strnlen,
    .strcnlen = (size32_t (*)(const void*, size32_t))strnlen,
    .strbnlen = (size32_t (*)(const void*, size32_t))strnlen,
};

#endif  // _EHBC_ENCODING_UCS2_H__
