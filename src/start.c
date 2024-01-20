/**
 * @file start.c
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Firmware entry point
 * @version r0
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stop.h"
#include "types.h"

void __attribute__((noreturn)) _start() {
    volatile uint32_t* test_var = (volatile uint32_t*)0x00010000;
    *test_var = 0xFFFFFFFF;
    while (1) {
        _stop_cpu(0);
    }
}
