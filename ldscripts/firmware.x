OUTPUT_FORMAT(elf32-m68k)

ENTRY(_start)

MEMORY {
    /* Bottom of the address space */
    FLASH(rx)   : ORIGIN = 0x00000000, LENGTH = 1M

    /* 1MiB above from the bottom */
    RAM(rwx)    : ORIGIN = 0x00100000, LENGTH = 4079M

    /* Last 16MiB of the address space */
    MMIO(rw)    : ORIGIN = 0xFF000000, LENGTH = 16M
}

SECTIONS {
    . = 0x00000000;
    .vector : {
        . = ALIGN(4);
        __vector_start = .;
        *(.vector);
        __vector_end = .;
    } >FLASH

    .text : {
        . = ALIGN(4);
        __text_start = .;
        KEEP(*(.init))
        KEEP(*(.text .text.*))
        __text_end = .;
    } >FLASH

    .ctors : {
        . = ALIGN(4);
        __ctors_start = .;
        *(.ctors)
        __ctors_end = .;
    } >FLASH

    .dtors : {
        . = ALIGN(4);
        __dtors_start = .;
        *(.dtors)
        __dtors_end = .;
    } >FLASH

    .rodata : {
        . = ALIGN(4);
        __rodata_start = .;
        KEEP(*(.rodata .rodata.*))
        __rodata_end = .;
    } >FLASH

    .data : {
        __data_init = LOADADDR(.data);
        . = ALIGN(4);
        __data_start = .;
        *(.data .data.*)
        __data_end = .;
    } >RAM AT >FLASH

    .fill : {
        FILL(0xFF);
    } >FLASH

    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        *(.noinit .noinit.*)
        __bss_end = .;
    } >RAM

    __heap_bottom = ORIGIN(RAM) + SIZEOF(.data) + SIZEOF(.bss);

    .mmio(NOLOAD) : {
        __mmio_start = .;
    } >MMIO
}
