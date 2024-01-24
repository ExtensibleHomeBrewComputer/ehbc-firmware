/**
 * @file cpu_stack.h
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Definition of the informations of the initial CPU stack.
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EHBC_ARCH_M68K_STACK_FRAME_H__
#define _EHBC_ARCH_M68K_STACK_FRAME_H__

#include <ehbc/types.h>

struct stack_frame0 {
    uint16_t sr;
    uint16_t pc_hi;
    uint16_t pc_lo;
    uint16_t vector_offset : 12;
    uint16_t frame_type : 4;
};

struct stack_frame2 {
    struct stack_frame0 frame0;
    uint16_t instr_hi;
    uint16_t instr_lo;
};

struct stack_frame9 {
    struct stack_frame0 frame0;
    uint16_t instr_hi;
    uint16_t instr_lo;
    uint16_t internal_register0;
    uint16_t internal_register1;
    uint16_t internal_register2;
    uint16_t internal_register3;
};

struct stack_frame10 {
    struct stack_frame0 frame0;
    uint16_t internal_register0;
    uint16_t special_status;
    uint16_t pipe_c;
    uint16_t pipe_b;
    uint16_t fault_address_hi;
    uint16_t fault_address_lo;
    uint16_t internal_register1;
    uint16_t internal_register2;
    uint16_t data_output_buf_hi;
    uint16_t data_output_buf_lo;
    uint16_t internal_register3;
    uint16_t internal_register4;
};

struct stack_frame11 {
    struct stack_frame0 frame0;
    uint16_t internal_register0;
    uint16_t special_status;
    uint16_t pipe_c;
    uint16_t pipe_b;
    uint16_t fault_address_hi;
    uint16_t fault_address_lo;
    uint16_t internal_register1;
    uint16_t internal_register2;
    uint16_t data_output_buf_hi;
    uint16_t data_output_buf_lo;
    uint16_t internal_register3;
    uint16_t internal_register4;
    uint16_t internal_register5;
    uint16_t internal_register6;
    uint16_t pipe_b_address_hi;
    uint16_t pipe_b_address_lo;
    uint16_t data_input_buf_hi;
    uint16_t data_input_buf_lo;
    uint16_t internal_register7;
    uint16_t internal_register8;
    uint16_t internal_register9;
    uint16_t internal_info : 12;
    uint16_t version_num : 4;
    uint16_t internal_register10;
    uint16_t internal_register11;
    uint16_t internal_register12;
    uint16_t internal_register13;
    uint16_t internal_register14;
    uint16_t internal_register15;
    uint16_t internal_register16;
    uint16_t internal_register17;
    uint16_t internal_register18;
    uint16_t internal_register19;
    uint16_t internal_register20;
    uint16_t internal_register21;
    uint16_t internal_register22;
    uint16_t internal_register23;
    uint16_t internal_register24;
    uint16_t internal_register25;
    uint16_t internal_register26;
    uint16_t internal_register27;
};

#endif  // _EHBC_ARCH_M68K_STACK_FRAME_H__
