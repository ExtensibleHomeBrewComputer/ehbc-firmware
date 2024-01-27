#include <ehbc/types.h>
#include <ehbc/printf.h>

void hexdump(const void* p, size_t len)
{
    if (p == NULL) return;

    size_t address = 0, row = 0;

    while (1) {
        if (address >= len) break;
        size_t nread = ((len - address) > 16) ? 16 : (len - address);

        printf("%08lX ", address);

        // print hexadecimal digits
        for (size_t i = 0; i < 16; i++) {
            if (i % 8 == 0) printf(" ");
            if (i < nread)
                printf(" %02X", ((uint8_t*)p)[16 * row + i]);
            else
                printf("   ");
        }

        // print characters if possible
        printf(" ");
        for (size_t i = 0; i < nread; i++) {
            uint8_t ch = ((uint8_t*)p)[16 * row + i];
            if (ch < 32 || ch > 126) {
                printf(".");
            } else {
                printf("%c", ch);
            }
        }

        printf("\r\n");
        address += 16;
        row++;
    }
}
