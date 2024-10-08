#include <ctype.h>

#include <stdint.h>

#define CNTRL   0x0001
#define PRINT   0x0002
#define SPACE   0x0004
#define BLANK   0x0008
#define GRAPH   0x0010
#define PUNCT   0x0020
#define ALNUM   0x0040
#define ALPHA   0x0080
#define UPPER   0x0100
#define LOWER   0x0200
#define DIGIT   0x0400
#define XDIGI   0x0800

static const uint16_t ctype_table[128] = {
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,  // 00h-07h
    0x0001, 0x000D, 0x0005, 0x0005, 0x0005, 0x0005, 0x0001, 0x0001,  // 08h-0Fh
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,  // 10h-17h
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,  // 18h-1Fh
    0x000E, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032,  // 20h-27h
    0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032,  // 28h-2Fh
    0x0C52, 0x0C52, 0x0C52, 0x0C52, 0x0C52, 0x0C52, 0x0C52, 0x0C52,  // 30h-37h
    0x0C52, 0x0C52, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032,  // 38h-3Fh
    0x0032, 0x09D2, 0x09D2, 0x09D2, 0x09D2, 0x09D2, 0x09D2, 0x01D2,  // 40h-47h
    0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2,  // 48h-4Fh
    0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2, 0x01D2,  // 50h-57h
    0x01D2, 0x01D2, 0x01D2, 0x0032, 0x0032, 0x0032, 0x0032, 0x0032,  // 58h-5Fh
    0x0032, 0x0AD2, 0x0AD2, 0x0AD2, 0x0AD2, 0x0AD2, 0x0AD2, 0x02D2,  // 60h-67h
    0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2,  // 68h-6Fh
    0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2, 0x02D2,  // 70h-77h
    0x02D2, 0x02D2, 0x02D2, 0x0032, 0x0032, 0x0032, 0x0032, 0x0001,  // 78h-7Fh
};

int isalnum(int c) {
    return !!(ctype_table[c] & ALNUM);
}

int isalpha(int c) {
    return !!(ctype_table[c] & ALPHA);
}

int iscntrl(int c) {
    return !!(ctype_table[c] & CNTRL);
}

int isdigit(int c) {
    return !!(ctype_table[c] & DIGIT);
}

int isgraph(int c) {
    return !!(ctype_table[c] & GRAPH);
}

int islower(int c) {
    return !!(ctype_table[c] & LOWER);
}

int isprint(int c) {
    return !!(ctype_table[c] & PRINT);
}

int ispunct(int c) {
    return !!(ctype_table[c] & PUNCT);
}

int isspace(int c) {
    return !!(ctype_table[c] & SPACE);
}

int isupper(int c) {
    return !!(ctype_table[c] & UPPER);
}

int isxdigit(int c) {
    return !!(ctype_table[c] & XDIGI);
}

int isblank(int c) {
    return !!(ctype_table[c] & BLANK);
}

int toupper(int c) {
    return (ctype_table[c] & LOWER) ? c - 'a' + 'A' : c;
}

int tolower(int c) {
    return (ctype_table[c] & UPPER) ? c - 'A' + 'a' : c;
}
