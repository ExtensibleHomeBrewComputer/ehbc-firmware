/**
 * @file stop.S
 * @author Minsu Kwon (kms1212g@gmail.com)
 * @brief Wrapper for stop instruction
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024 Minsu Kwon (kms1212g@gmail.com)
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

.org 0

.global cpu_reset
.global cpu_set_interrupt_mask
.global cpu_set_user_mode
.global cpu_set_master_mode
.global cpu_set_interrupt_mode
.global cpu_set_control_register
.global cpu_get_control_register

.text
/* void(void) */
cpu_reset:
    reset

/* void(const uint8_t) */
cpu_set_interrupt_mask:
    /* prolog, */
    link.w      %fp,#-4
    move.l      %fp@(8),%d0
    /* %sr |= ((%d0 & 7) << 8);  // side: %d1 */
    andi.b      #7,%d5
    lsl.w       #8,%d0
    move.w      %sr,%d1
    or.w        %d1,%d0
    move.w      %d0,%sr
    /* epilog */
    nop
    unlk        %fp
    rts

/* void(void) */
cpu_set_user_mode:
    /* prolog, */
    link.w      %fp,#0
    /* %sr |= (1 << 13); */
    move.w      %sr,%d0
    bset        #13,%d0
    move.w      %d0,%sr
    /* epilog */
    unlk        %fp
    rts

/* void(void) */
cpu_set_master_mode:
    /* prolog, */
    link.w      %fp,#0
    /* %sr |= (1 << 12); */
    move.w      %sr,%d0
    bset        #12,%d0
    move.w      %d0,%sr
    /* epilog */
    unlk        %fp
    rts

/* void(void) */
cpu_set_interrupt_mode:
    /* prolog, */
    link.w      %fp,#0
    /* %sr |= (1 << 12); */
    move.w      %sr,%d0
    bclr        #12,%d0
    move.w      %d0,%sr
    /* epilog */
    unlk        %fp
    rts
