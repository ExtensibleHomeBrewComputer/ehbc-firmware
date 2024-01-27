/**
 * @file types.h
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Type definitions
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EHBC_TYPES_H__
#define _EHBC_TYPES_H__

#include <stdint.h>
#include <stddef.h>

typedef int         error_t;

typedef uint32_t    size32_t;
typedef uint16_t    size16_t;
typedef int32_t     ssize32_t;
typedef int16_t     ssize16_t;

typedef volatile uint8_t hwreg8_t;
typedef volatile uint16_t hwreg16_t;
typedef volatile uint32_t hwreg32_t;
typedef volatile uint64_t hwreg64_t;

typedef uint32_t    addr32_t;
typedef uint64_t    addr64_t;

typedef void        (*vfunc_t)();
typedef void*       ptr_t;
#define PTR(ptr)    (ptr_t)(ptr)

#endif  // _EHBC_TYPES_H__
