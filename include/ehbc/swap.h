#ifndef _EHBC_SWAP_H__
#define _EHBC_SWAP_H__

#include <ehbc/types.h>

#define byteswap16(v)   (((v & 0xFF00) >> 8) | ((v & 0x00FF) << 8))
#define byteswap32(v)   (((v & 0xFF000000) >> 24) | ((v & 0x00FF0000) >> 8) | ((v & 0x0000FF00) << 8) | ((v & 0x000000FF) << 24))

#endif  // _EHBC_SWAP_H__
