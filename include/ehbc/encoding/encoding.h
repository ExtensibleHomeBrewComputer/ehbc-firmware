#ifndef _EHBC_ENCODING_ENCODING_H__
#define _EHBC_ENCODING_ENCODING_H__

#include <ehbc/types.h>
#include <ehbc/struct.h>

typedef uint32_t    uraw_t;

typedef struct {
    size32_t (*decode)(const void* encoded, uraw_t* decoded, size32_t size);
    size32_t (*encode)(uraw_t decoded, void* encoded, size32_t size);

    size32_t (*chrbnlen)(const void* seq, size32_t size);
    size32_t (*strcnlen)(const void* str, size32_t size);
    size32_t (*strbnlen)(const void* str, size32_t size);
} EncodingTrait;

uraw_t memberof(Encoding, ctolower)(uraw_t c);
uraw_t memberof(Encoding, ctoupper)(uraw_t c);

size32_t memberof(Encoding, convert)(const EncodingTrait* tdest, void* dest, size32_t dsize, const EncodingTrait* tsrc, const void* src, size32_t ssize);

int memberof(Encoding, strncmp)(const EncodingTrait* trait, const void* str1, const void* str2, size32_t size);
int memberof(Encoding, strnicmp)(const EncodingTrait* trait, const void* str1, const void* str2, size32_t size);
void* memberof(Encoding, strncpy)(const EncodingTrait* trait, void* dest, const void* src, size32_t size);


#endif  // _EHBC_ENCODING_ENCODING_H__
