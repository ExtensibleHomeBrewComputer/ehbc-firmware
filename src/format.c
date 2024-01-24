#include <ehbc/format.h>

#include <stdarg.h>

#include <ehbc/types.h>

#define FLAG_JUSTIFY_LEFT   0x01
#define FLAG_FORCE_SIGN     0x02
#define FLAG_PAD_SIGN       0x04
#define FLAG_INSERT_PREFIX  0x08
#define FLAG_ZERO_PADDING   0x10

#define FLAGS_REQUIRED      4
#define WIDTH_REQUIRED      3
#define LENGTH_REQUIRED     2
#define SPECIFIER_REQUIRED  1

#define WIDTH_FROMARG       -1

#define LENGTH_INT          0
#define LENGTH_CHAR         1
#define LENGTH_SHORT        2
#define LENGTH_LONG         3
#define LENGTH_LONGLONG     4
#define LENGTH_INTMAX       5
#define LENGTH_SIZE         6
#define LENGTH_PTRDIFF      7
#define LENGTH_LONGDOUBLE   8

static const char hex_char_table_upper[] = "0123456789ABCDEF";
static const char hex_char_table_lower[] = "0123456789abcdef";

#define throw_error()               \
    do {                            \
        bytes_written = -1;         \
        goto finish;                \
    } while(0)

#define copy_char(ch, dest)         \
    if (len-- > 0) {                \
        *(dest) = ch;               \
        bytes_written++;            \
    } else {                        \
        throw_error();              \
    }

/* TODO: finish implementation */
int vformat(char* buf, size32_t len, const char* fmt, va_list ap)
{
    int result = 0;
    uint8_t state = 0;
    char flags = '\0';
    int width = 0;
    char length = 0;
    int bytes_written = 0;

    for (; *fmt != 0; fmt++) {
        if (state == FLAGS_REQUIRED) {
            switch (*fmt) {
                case '-':
                    flags |= FLAG_JUSTIFY_LEFT;
                    break;
                case '+':
                    flags |= FLAG_FORCE_SIGN;
                    break;
                case ' ':
                    flags |= FLAG_PAD_SIGN;
                    break;
                case '0':
                    flags |= FLAG_ZERO_PADDING;
                    break;
                case '#':
                    flags |= FLAG_INSERT_PREFIX;
                    break;
                default:
                    state--;
                    break;
            }
        }

        if (state == WIDTH_REQUIRED) {
            if (*fmt == '*') {
                width = WIDTH_FROMARG;
                continue;
            }

            char digit = *fmt - '0';
            if (digit < 10 && digit >= 0) {
                width *= 10;
                width += digit;
                continue;
            }
            state--;
        }

        if (state == LENGTH_REQUIRED) {
            switch (*fmt) {
                case 'h':
                    if (*(fmt + 1) == 'h') {
                        fmt++;
                        length = LENGTH_CHAR;
                    } else {
                        length = LENGTH_SHORT;
                    }
                    break;
                case 'l':
                    if (*(fmt + 1) == 'l') {
                        fmt++;
                        length = LENGTH_LONGLONG;
                    } else {
                        length = LENGTH_LONG;
                    }
                    break;
                case 'j':
                    length = LENGTH_INTMAX;
                    break;
                case 'z':
                    length = LENGTH_SIZE;
                    break;
                case 't':
                    length = LENGTH_PTRDIFF;
                    break;
                case 'L':
                    length = LENGTH_LONGDOUBLE;
                    break;
                default:
                    length = LENGTH_INT;
                    state--;
                    break;
            }
        }

        if (state == SPECIFIER_REQUIRED) {
            switch (*fmt) {
                case 'd':
                case 'i':
                    break;
                case 'u':
                    break;
                case 'o':
                    break;
                case 'x': {
                    intmax_t value;
                    switch (length) {
                        case LENGTH_CHAR:
                        case LENGTH_SHORT:
                        case LENGTH_INT:
                            value = va_arg(ap, int);
                            break;
                        case LENGTH_LONG:
                            value = va_arg(ap, long);
                            break;
                        case LENGTH_LONGLONG:
                            value = va_arg(ap, long long);
                            break;
                        case LENGTH_INTMAX:
                            value = va_arg(ap, intmax_t);
                            break;
                        case LENGTH_SIZE:
                            value = va_arg(ap, size_t);
                            break;
                        case LENGTH_PTRDIFF:
                            value = va_arg(ap, int);
                            break;
                        default:
                            throw_error();
                    }
                    char cvt_buf_rev[32];
                    char padding_char = '\0';
                    for (int i = 0; i < sizeof(cvt_buf_rev); i++) {
                        cvt_buf_rev[i] = value & 0xF;
                        value >>= 4;
                    }
                    if (flags & FLAG_INSERT_PREFIX) {
                        copy_char('0', buf++);
                        copy_char('x', buf++);
                    }
                    for (int i = sizeof(cvt_buf_rev) - 1; i >= 0; i--) {
                        if (i == width - 1) {
                            if (flags & FLAG_ZERO_PADDING) {
                                padding_char = '0';
                            } else {
                                padding_char = ' ';
                            }
                        }

                        if (cvt_buf_rev[i] == 0) {
                            if (padding_char != '\0') {
                                copy_char(padding_char, buf++);
                            }
                        } else {
                            copy_char(hex_char_table_lower[(int)cvt_buf_rev[i]], buf++);
                        }
                    }
                    break;
                }
                case 'X':
                    break;
                case 'f':
                    break;
                case 'F':
                    break;
                case 'e':
                    break;
                case 'E':
                    break;
                case 'g':
                    break;
                case 'G':
                    break;
                case 'a':
                    break;
                case 'A':
                    break;
                case 'c':
                    copy_char((char)va_arg(ap, int), buf++);
                    break;
                case 's': {
                    const char* str = va_arg(ap, const char*);
                    if (width == WIDTH_FROMARG) {
                        width = va_arg(ap, int);
                    }
                    for (; *str != 0; str++) {
                        if (--width != 0) {
                            copy_char(*str, buf++);
                        } else {
                            throw_error();
                        }
                    }
                    break;
                }
                case 'p':
                    break;
                case 'n':
                    break;
                default:
                    bytes_written = -1;
                    goto finish;
            }
            state = 0;
            flags = '\0';
            width = 0;
            length = 0;
            continue;
        }

        if (!state) {
            switch (*fmt) {
                case '\0':
                    goto finish;
                case '%':
                    if (*(fmt + 1) != '%') {
                        state = FLAGS_REQUIRED;
                        break;
                    }
                default:
                    copy_char(*fmt, buf++);
                    break;
            }
        }
    }

finish:
    if (len-- > 0) {
        *buf = '\0';
        bytes_written++;
    } else {
        bytes_written = -1;
    }

    return bytes_written;
}
