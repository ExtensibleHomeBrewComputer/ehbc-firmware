/**
 * @file stop.h
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Stop instruction wrapper
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms12g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EHBC_ASM_H__
#define _EHBC_ASM_H__

#include <ehbc/types.h>

void cpu_reset(void);
void cpu_set_interrupt_mask(const uint8_t mask);
void cpu_set_user_mode(void);
void cpu_set_master_mode(void);
void cpu_set_interrupt_mode(void);

#endif  // _EHBC_ASM_H__
