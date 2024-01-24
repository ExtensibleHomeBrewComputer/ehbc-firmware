/**
 * @file vector.c
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Default exception vector declarations
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

#include <ehbc/stack.h>
#include <ehbc/types.h>

#include "arch/m68k/vector.h"

extern void _start(void);

__attribute__((interrupt_handler))
void default_vector_handler(const struct stack_frame0* info)
{
    for (;;) {}
}

__attribute__((section(".vector")))
const struct vector default_vector = {
    .initial_sp                     = PTR(STACK_INITIAL_TOP),
    .initial_pc                     = _start,

    .bus_error                      = default_vector_handler,
    .address_error                  = default_vector_handler,
    .illegal_instruction            = default_vector_handler,
    .divide_by_zero                 = default_vector_handler,
    .check_instruction              = default_vector_handler,
    .trap_instruction               = default_vector_handler,
    .privilege_violation            = default_vector_handler,
    .format_error                   = default_vector_handler,
    .uninitialized_interrupt        = default_vector_handler,
    .spurious_interrupt             = default_vector_handler,
    .fpcp_branch_or_set_condition   = default_vector_handler,
    .fpcp_inexact_result            = default_vector_handler,
    .fpcp_divde_by_zero             = default_vector_handler,
    .fpcp_underflow                 = default_vector_handler,
    .fpcp_operand_error             = default_vector_handler,
    .fpcp_overflow                  = default_vector_handler,
    .fpcp_nan_result                = default_vector_handler,
    .mmu_config_error               = default_vector_handler,

    .trap                           = { default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler },

    .autovector                     = { default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler,
                                        default_vector_handler },
};
