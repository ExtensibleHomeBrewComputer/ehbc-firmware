#ifndef _EHBC_SECTION_H__
#define _EHBC_SECTION_H__

#include <ehbc/types.h>

extern const int __vector_start;
extern const int __vector_end;
extern const int __text_start;
extern const int __text_end;
extern const int __rodata_start;
extern const int __rodata_end;
extern const int __ctors_start;
extern const int __ctors_end;
extern const int __dtors_start;
extern const int __dtors_end;
extern const int __data_init;
extern const int __data_start;
extern const int __data_end;
extern const int __bss_start;
extern const int __bss_end;

#define VECTOR_START    (&__vector_start)
#define VECTOR_END      (&__vector_end)
#define TEXT_START      (&__text_start)
#define TEXT_END        (&__text_end)
#define RODATA_START    (&__rodata_start)
#define RODATA_END      (&__rodata_end)
#define CTORS_START     (&__ctors_start)
#define CTORS_END       (&__ctors_end)
#define DTORS_START     (&__dtors_start)
#define DTORS_END       (&__dtors_end)
#define DATA_INIT       (&__data_init)
#define DATA_START      (&__data_start)
#define DATA_END        (&__data_end)
#define BSS_START       (&__bss_start)
#define BSS_END         (&__bss_end)

#endif  // _EHBC_SECTION_H__
