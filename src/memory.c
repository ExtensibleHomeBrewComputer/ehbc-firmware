#include <ehbc/memory.h>
#include <ehbc/stack.h>
#include <ehbc/string.h>
#include <stdalign.h>

#define BLOCK_FREE      0
#define BLOCK_ALLOCATED 1
#define BLOCK_ALLOCEND  2

#define NOT_FOUND UINT32_MAX

static uint8_t* alloc_table;
static void* alloc_area_base;
static uint32_t block_count;

static uint32_t detect_memory_size(void)
{
    return 16 * MiB;
}

static void set_table_block(uint8_t value, uint32_t idx)
{
    uint8_t dbit_idx = idx % 4;
    uint32_t byte_idx = idx / 4;
    alloc_table[byte_idx] &=  ~(3 << (dbit_idx * 2));
    alloc_table[byte_idx] |=  (value & 3) << (dbit_idx * 2);
}

static uint8_t get_table_block(uint32_t idx)
{
    uint8_t dbit_idx = idx % 4;
    uint32_t byte_idx = idx / 4;
    return (alloc_table[byte_idx] >> (dbit_idx * 2)) & 3;
}

__attribute__((constructor))
static void __setup_memory_management(void)
{
    alloc_table = (uint8_t*)STACK_TOP;

    uint32_t msize = detect_memory_size() - (uint32_t)STACK_TOP;
    block_count = msize / MALLOC_BLOCKSIZE;
    uint32_t table_size = block_count / 4;
    block_count -= (table_size + MALLOC_BLOCKSIZE - 1) / MALLOC_BLOCKSIZE;

    alloc_area_base = (uint8_t*)STACK_TOP + table_size + MALLOC_BLOCKSIZE - ((uint32_t)alloc_area_base & (MALLOC_BLOCKSIZE - 1));

    if ((uint32_t)alloc_area_base >= msize) {
        while (1) {}
    }

    memset(alloc_table, 0, table_size);
}

void* mem_alloc(size32_t size)
{
    uint32_t free_block_start = NOT_FOUND;
    uint32_t free_continuous_blocks = 0;
    uint32_t required_free_blocks = (size + MALLOC_BLOCKSIZE - 1) / MALLOC_BLOCKSIZE;

    for (uint32_t current_block = 0; current_block < block_count; current_block++) {
        if (get_table_block(current_block) == 0) {
            if (free_block_start == NOT_FOUND) {
                free_block_start = current_block;
            }
            free_continuous_blocks++;
            if (free_continuous_blocks >= required_free_blocks) {
                for (uint32_t i = 0; i < free_continuous_blocks - 1; i++) {
                    set_table_block(BLOCK_ALLOCATED, free_block_start + i);
                }
                set_table_block(BLOCK_ALLOCEND, free_block_start + free_continuous_blocks - 1);
                return (uint8_t*)alloc_area_base + MALLOC_BLOCKSIZE * free_block_start;
            }
        } else {
            free_block_start = NOT_FOUND;
            free_continuous_blocks = 0;
        }
    }

    return NULL;
}

void mem_free(void* ptr)
{
    if ((uint32_t)ptr < (uint32_t)alloc_area_base) {
        return;
    }

    uint32_t block_idx = ((uint32_t)ptr - (uint32_t)alloc_area_base) / MALLOC_BLOCKSIZE;
    uint8_t block_status = 0;

    do {
        block_status = get_table_block(block_idx);
        set_table_block(0, block_idx++);
    } while (block_status != BLOCK_ALLOCEND);
}
