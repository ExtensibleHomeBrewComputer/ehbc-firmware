    ORG         0

    PUBLIC      __init_arch
    PUBLIC      __istack_end
    PUBLIC      __stack_end
    PUBLIC      exception_vector

    SECTION     .text.startup
_start::
    LEA         __istack_end,SP         ; interrupt stack pointer

    ORI.W       #$1000,SR               ; switch to master state

    LEA         __stack_end,SP          ; master stack pointer
    MOVEA.L     SP,A6                   ; frame pointer

    ; init memory

    ; init exception vector
    MOVE.L      #exception_vector,D0
    MOVEC.L     D0,VBR

    ; Jump to __init_arch
    BRA         __init_arch

; Test bank
;   Registers:
;     D0: Bank index (0 to 7)
;   Return:
;     D0: Bank size
;   Modifies: D0, D1, A0, A1
test_bank:
    CLR.L       D1
    RTS

__helloworld_str:
    DC.B "Hello, World!\r\n", 0
