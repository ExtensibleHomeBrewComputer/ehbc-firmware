#include <ehbc/printf.h>

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

static size32_t u32toa(uint32_t value, char* buf, size32_t len, char radix)
{
    size32_t width = 0;
    switch (radix) {
        case 'o':
            do {
                buf[width++] = hex_char_table_upper[value & 0x07];
                value >>= 3;
            } while (width < len && value != 0);
            break;
        case 'd':
            do {
                buf[width++] = (value % 10) + '0';
                value /= 10;
            } while (width < len && value != 0);
            break;
        case 'x':
            do {
                buf[width++] = hex_char_table_lower[value & 0xF];
                value >>= 4;
            } while (width < len && value != 0);
            break;
        case 'X':
            do {
                buf[width++] = hex_char_table_upper[value & 0xF];
                value >>= 4;
            } while (width < len && value != 0);
            break;
        default:
            buf[width++] = '0';
            break;
    }
    return width;
}

/* TODO: finish implementation */
int vsnprintf(char* buf, size32_t len, const char* fmt, va_list ap)
{
    int result = 0;
    uint8_t state = 0;
    char flags = '\0';
    int width = 0;
    char length = 0;
    int bytes_written = 0;
    char integer_output = '\0';

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
                        length = LENGTH_CHAR;
                        fmt += 2;
                    } else {
                        length = LENGTH_SHORT;
                        fmt++;
                    }
                    break;
                case 'l':
                    if (*(fmt + 1) == 'l') {
                        length = LENGTH_LONGLONG;
                        fmt += 2;
                    } else {
                        length = LENGTH_LONG;
                        fmt++;
                    }
                    break;
                case 'j':
                    length = LENGTH_INTMAX;
                    fmt++;
                    break;
                case 'z':
                    length = LENGTH_SIZE;
                    fmt++;
                    break;
                case 't':
                    length = LENGTH_PTRDIFF;
                    fmt++;
                    break;
                case 'L':
                    length = LENGTH_LONGDOUBLE;
                    fmt++;
                    break;
                default:
                    length = LENGTH_INT;
                    break;
            }
            state--;
        }

        if (state == SPECIFIER_REQUIRED) {
            switch (*fmt) {
                case 'i':
                    integer_output = 'd';
                    break;
                case 'u':
                case 'd':
                case 'o':
                case 'x':
                case 'X': 
                    integer_output = *fmt;
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
                            break;
                        }
                    }
                    break;
                }
                case 'p':
                    break;
                case 'n':
                    break;
                default:
                    throw_error();
            }

            if (integer_output) {
                uint32_t value;
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
                    case LENGTH_INTMAX:
                    case LENGTH_SIZE:
                    case LENGTH_PTRDIFF:
                    default:
                        throw_error();
                }
                switch (integer_output) {
                    case 'd':
                        if ((int32_t)value < 0) {
                            value = -(int32_t)value;
                            copy_char('-', buf++);
                        } else if (flags & FLAG_FORCE_SIGN) {
                            copy_char('+', buf++);
                        } else if (flags & FLAG_PAD_SIGN) {
                            copy_char(' ', buf++);
                        }
                        break;
                    case 'u':
                        integer_output = 'd';
                        break;
                }
                char cvt_buf[10];
                size32_t cvt_width = u32toa(value, cvt_buf, sizeof(cvt_buf), integer_output);
                if (width == 0) {
                    width = cvt_width;
                }
                for (; width > 0; width--) {
                    if (width > cvt_width) {
                        if (flags & FLAG_ZERO_PADDING) {
                            copy_char('0', buf++);
                        } else {
                            copy_char(' ', buf++);
                        }
                    } else {
                        char ch = cvt_buf[width - 1];
                        copy_char(ch, buf++);
                    }
                }
            }

            state = 0;
            flags = '\0';
            width = 0;
            length = 0;
            integer_output = '\0';
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
    } else {
        bytes_written = -1;
    }

    return bytes_written;
}
