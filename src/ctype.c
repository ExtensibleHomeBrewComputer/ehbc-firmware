#include <ehbc/ctype.h>

#include <ehbc/types.h>

#define ALNUM 0x0001
#define ALPHA 0x0002
#define BLANK 0x0004
#define CNTRL 0x0008
#define DIGIT 0x0010
#define GRAPH 0x0020
#define LOWER 0x0040
#define PRINT 0x0080
#define PUNCT 0x0100
#define SPACE 0x0200
#define UPPER 0x0400
#define XDIGIT 0x0800

static const uint16_t char_table[] = {
    /* \x00 */ CNTRL,
    /* \x01 */ CNTRL,
    /* \x02 */ CNTRL,
    /* \x03 */ CNTRL,
    /* \x04 */ CNTRL,
    /* \x05 */ CNTRL,
    /* \x06 */ CNTRL,
    /* \x07 */ CNTRL,
    /* \x08 */ CNTRL,
    /* \x09 */ CNTRL | SPACE | BLANK,
    /* \x0A */ CNTRL | SPACE,
    /* \x0B */ CNTRL,
    /* \x0C */ CNTRL,
    /* \x0D */ CNTRL,
    /* \x0E */ CNTRL,
    /* \x0F */ CNTRL,
    /* \x10 */ CNTRL,
    /* \x11 */ CNTRL,
    /* \x12 */ CNTRL,
    /* \x13 */ CNTRL,
    /* \x14 */ CNTRL,
    /* \x15 */ CNTRL,
    /* \x16 */ CNTRL,
    /* \x17 */ CNTRL,
    /* \x18 */ CNTRL,
    /* \x19 */ CNTRL,
    /* \x1A */ CNTRL,
    /* \x1B */ CNTRL,
    /* \x1C */ CNTRL,
    /* \x1D */ CNTRL,
    /* \x1E */ CNTRL,
    /* \x1F */ CNTRL,
    /*      */ PRINT | SPACE | BLANK,
    /*   !  */ PRINT | GRAPH | PUNCT,
    /*   "  */ PRINT | GRAPH | PUNCT,
    /*   #  */ PRINT | GRAPH | PUNCT,
    /*   $  */ PRINT | GRAPH | PUNCT,
    /*   %  */ PRINT | GRAPH | PUNCT,
    /*   &  */ PRINT | GRAPH | PUNCT,
    /*   '  */ PRINT | GRAPH | PUNCT,
    /*   (  */ PRINT | GRAPH | PUNCT,
    /*   )  */ PRINT | GRAPH | PUNCT,
    /*   *  */ PRINT | GRAPH | PUNCT,
    /*   +  */ PRINT | GRAPH | PUNCT,
    /*   ,  */ PRINT | GRAPH | PUNCT,
    /*   -  */ PRINT | GRAPH | PUNCT,
    /*   .  */ PRINT | GRAPH | PUNCT,
    /*   /  */ PRINT | GRAPH | PUNCT,
    /*   0  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   1  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   2  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   3  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   4  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   5  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   6  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   7  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   8  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   9  */ PRINT | GRAPH | ALNUM | DIGIT | XDIGIT,
    /*   :  */ PRINT | GRAPH | PUNCT,
    /*   ;  */ PRINT | GRAPH | PUNCT,
    /*   <  */ PRINT | GRAPH | PUNCT,
    /*   =  */ PRINT | GRAPH | PUNCT,
    /*   >  */ PRINT | GRAPH | PUNCT,
    /*   ?  */ PRINT | GRAPH | PUNCT,
    /*   @  */ PRINT | GRAPH | PUNCT,
    /*   A  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   B  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   C  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   D  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   E  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   F  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER | XDIGIT,
    /*   G  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   H  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   I  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   J  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   K  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   L  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   M  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   N  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   O  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   P  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   Q  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   R  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   S  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   T  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   U  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   V  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   W  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   X  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   Y  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   Z  */ PRINT | GRAPH | ALNUM | ALPHA | UPPER,
    /*   [  */ PRINT | GRAPH | PUNCT,
    /*   \  */ PRINT | GRAPH | PUNCT,
    /*   ]  */ PRINT | GRAPH | PUNCT,
    /*   ^  */ PRINT | GRAPH | PUNCT,
    /*   _  */ PRINT | GRAPH | PUNCT,
    /*   `  */ PRINT | GRAPH | PUNCT,
    /*   a  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   b  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   c  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   d  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   e  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   f  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER | XDIGIT,
    /*   g  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   h  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   i  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   j  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   k  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   l  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   m  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   n  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   o  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   p  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   q  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   r  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   s  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   t  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   u  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   v  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   w  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   x  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   y  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   z  */ PRINT | GRAPH | ALNUM | ALPHA | LOWER,
    /*   {  */ PRINT | GRAPH | PUNCT,
    /*   |  */ PRINT | GRAPH | PUNCT,
    /*   }  */ PRINT | GRAPH | PUNCT,
    /*   ~  */ PRINT | GRAPH | PUNCT,
    /* \x7F */ CNTRL,
};

int isalnum(int c)
{
    return c < 0x80 && (char_table[c] & 1);
}

int isalpha(int c)
{
    return c < 0x80 && ((char_table[c] >> 1) & 1);
}

int isblank(int c)
{
    return c < 0x80 && ((char_table[c] >> 2) & 1);
}

int iscntrl(int c)
{
    return c < 0x80 && ((char_table[c] >> 3) & 1);
}

int isdigit(int c)
{
    return c < 0x80 && ((char_table[c] >> 4) & 1);
}

int isgraph(int c)
{
    return c < 0x80 && ((char_table[c] >> 5) & 1);
}

int islower(int c)
{
    return c < 0x80 && ((char_table[c] >> 6) & 1);
}

int isprint(int c)
{
    return c < 0x80 && ((char_table[c] >> 7) & 1);
}

int ispunct(int c)
{
    return c < 0x80 && ((char_table[c] >> 8) & 1);
}

int isspace(int c)
{
    return c < 0x80 && ((char_table[c] >> 9) & 1);
}

int isupper(int c)
{
    return c < 0x80 && ((char_table[c] >> 10) & 1);
}

int isxdigit(int c)
{
    return c < 0x80 && ((char_table[c] >> 11) & 1);
}

int tolower(int c)
{
    return (c < 0x80 && isupper(c)) ? (c + 0x20) : c;
}

int toupper(int c)
{
    return (c < 0x80 && islower(c)) ? (c - 0x20) : c;
}
