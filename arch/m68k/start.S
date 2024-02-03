.org 0

#include <ehbc/section.h>
#include <ehbc/stack.h>

.global         main
.global          _start

.text
/* firmware entry point */
_start:
    /* copy .data section */
    movea.l     #__data_start,%a0
    movea.l     #__data_init,%a1
0:
    cmpa.l      #__data_end,%a0
    bge         .skip_data
    move.l      %a1@+,%a0@+
    bra         0b
.skip_data:

    /* set stack pointer */
    movea.l     #__stack_top,%a7

    /* run constructors */
    movea.l     #__ctors_start,%a2
0:
    cmpa.l      #__ctors_end,%a2
    bge         .skip_ctors
    movea.l     %a2@+,%a3
    jsr         %a3@
    bra         0b
.skip_ctors:

    /* jump to main */
    jsr         main

    /* infinite loop */
0:
    bra         0b
