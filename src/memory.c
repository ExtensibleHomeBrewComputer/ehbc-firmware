#include <ehbc/memory.h>

ptr_t __heap_top_ptr;


__attribute__((constructor))
static void __heap_init(void) {
    __heap_top_ptr = HEAP_BOTTOM;
}

void* mem_alloc(size32_t size) {
    void* allocated_ptr = __heap_top_ptr;
    __heap_top_ptr = (uint8_t*)HEAP_TOP + size;
    return allocated_ptr;
}

void mem_free(void* ptr) {
    /* TODO */
}

