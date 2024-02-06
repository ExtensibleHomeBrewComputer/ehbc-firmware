#ifndef _EHBC_ENCODING_UTF8_H__
#define _EHBC_ENCODING_UTF8_H__

#include <ehbc/types.h>
#include <ehbc/encoding/encoding.h>
#include <ehbc/struct.h>

typedef uint8_t UTF8;

#define utf8(str) ((UTF8*)str)

size32_t memberof(UTF8, decode)(const void* encoded, uraw_t* decoded, size32_t size);
size32_t memberof(UTF8, encode)(uraw_t decoded, void* encoded, size32_t size);

size32_t memberof(UTF8, chrbnlen)(const void* seq, size32_t size);
size32_t memberof(UTF8, strcnlen)(const void* str, size32_t size);
size32_t memberof(UTF8, strbnlen)(const void* str, size32_t size);

static const EncodingTrait impl(UTF8, EncodingTrait) = {
    .decode = memberof(UTF8, decode),
    .encode = memberof(UTF8, encode),
    .chrbnlen = memberof(UTF8, chrbnlen),
    .strcnlen = memberof(UTF8, strcnlen),
    .strbnlen = memberof(UTF8, strbnlen),
};

#endif  // _EHBC_ENCODING_UTF8_H__
