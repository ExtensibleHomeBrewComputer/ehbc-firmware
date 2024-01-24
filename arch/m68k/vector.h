/**
 * @file vector.h
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Definitions about the MC68030 exception vector
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EHBC_VECTOR_VECTOR_H__
#define _EHBC_VECTOR_VECTOR_H__

#include <ehbc/types.h>

#include "arch/m68k/stack_frame.h"

typedef void (*vector_handler_t)(const struct stack_frame0* info);

struct vector {
    ptr_t               initial_sp;
    vfunc_t             initial_pc;
    vector_handler_t    bus_error;
    vector_handler_t    address_error;
    vector_handler_t    illegal_instruction;
    vector_handler_t    divide_by_zero;
    vector_handler_t    check_instruction;
    vector_handler_t    trap_instruction;
    vector_handler_t    privilege_violation;
    vector_handler_t    trace;
    vector_handler_t    line_1010_emu;
    vector_handler_t    line_1111_emu;
    uint32_t            __reserved1;
    vector_handler_t    coproc_protocol_volation;
    vector_handler_t    format_error;
    vector_handler_t    uninitialized_interrupt;
    uint32_t            __reserved2[8];
    vector_handler_t    spurious_interrupt;
    vector_handler_t    autovector[7];
    vector_handler_t    trap[16];
    vector_handler_t    fpcp_branch_or_set_condition;
    vector_handler_t    fpcp_inexact_result;
    vector_handler_t    fpcp_divde_by_zero;
    vector_handler_t    fpcp_underflow;
    vector_handler_t    fpcp_operand_error;
    vector_handler_t    fpcp_overflow;
    vector_handler_t    fpcp_nan_result;
    uint32_t            __reserved3;
    vector_handler_t    mmu_config_error;
    uint32_t            __reserved4[7];
    ptr_t               userdef[192];
};

extern const struct vector default_vector;

#endif  // _EHBC_VECTOR_VECTOR_H__
