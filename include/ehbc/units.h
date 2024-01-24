/**
 * @file units.h
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief SI prefix (+ unit) definitions
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EHBC_UNITS_H__
#define _EHBC_UNITS_H__

#include <ehbc/types.h>

#define ki 1024
#define Mi (1024 * ki)
#define Gi (1024 * Mi)

#define kiB (ki * sizeof(uint8_t))
#define MiB (Mi * sizeof(uint8_t))
#define GiB (Gi * sizeof(uint8_t))

#endif  // _EHBC_UNITS_H__
