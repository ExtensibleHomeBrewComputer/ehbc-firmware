#ifndef _EHBC_ENCODING_UCS2_H__
#define _EHBC_ENCODING_UCS2_H__

#include <ehbc/types.h>
#include <ehbc/encoding/encoding.h>
#include <ehbc/struct.h>

typedef uint16_t UCS2;

size32_t memberof(UCS2, decode)(const void* encoded, uraw_t* decoded, size32_t size);
size32_t memberof(UCS2, encode)(uraw_t decoded, void* encoded, size32_t size);

size32_t memberof(UCS2, chrbnlen)(const void* seq, size32_t size);
size32_t memberof(UCS2, strcnlen)(const void* str, size32_t size);
size32_t memberof(UCS2, strbnlen)(const void* str, size32_t size);

static const EncodingTrait impl(UCS2, EncodingTrait) = {
    .decode = memberof(UCS2, decode),
    .encode = memberof(UCS2, encode),
    .chrbnlen = memberof(UCS2, chrbnlen),
    .strcnlen = memberof(UCS2, strcnlen),
    .strbnlen = memberof(UCS2, strbnlen),
};

#endif  // _EHBC_ENCODING_UCS2_H__
