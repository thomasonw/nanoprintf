/* https://github.com/nothings/stb/blob/master/docs/stb_howto.txt */
#ifndef NANOPRINTF_IMPLEMENTATION

#ifndef NANOPRINTF_H_INCLUDED
#define NANOPRINTF_H_INCLUDED

#ifndef NANOPRINTF_USE_C99_FORMAT_SPECIFIERS
#error NANOPRINTF_USE_C99_FORMAT_SPECIFIERS must be #defined to 0 or 1
#endif

#ifndef NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS
#error NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS must be #defined to 0 or 1
#endif

#ifdef NANOPRINTF_VISIBILITY_STATIC
#define NPF_INTERFACE_DEF static
#else
#define NPF_INTERFACE_DEF extern
#endif

#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
#ifdef __cplusplus
#if __cplusplus < 201103L
#error nanoprintf float support requires fixed-width types from c++11 or later.
#endif
#else
#if __STDC_VERSION__ < 199409L
#error nanoprintf float support requires fixed-width types from c99 or later.
#endif
#endif
#endif

#include <stdarg.h>
#include <stddef.h>

#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Public API */

NPF_INTERFACE_DEF int npf_snprintf(char *buffer, size_t bufsz,
                                   const char *format, ...);
NPF_INTERFACE_DEF int npf_vsnprintf(char *buffer, size_t bufsz,
                                    char const *format, va_list vlist);

enum { NPF_EOF = -1 };
typedef int (*npf_putc)(int c, void *ctx);
NPF_INTERFACE_DEF int npf_pprintf(npf_putc pc, void *pc_ctx, char const *format,
                                  ...);
NPF_INTERFACE_DEF int npf_vpprintf(npf_putc pc, void *pc_ctx,
                                   char const *format, va_list vlist);

/* Internal */

typedef enum {
    NPF_FMT_SPEC_FIELD_WIDTH_NONE,
    NPF_FMT_SPEC_FIELD_WIDTH_LITERAL,
    NPF_FMT_SPEC_FIELD_WIDTH_STAR
} npf__format_spec_field_width_t;

typedef enum {
    NPF_FMT_SPEC_PRECISION_NONE,
    NPF_FMT_SPEC_PRECISION_LITERAL,
    NPF_FMT_SPEC_PRECISION_STAR
} npf__format_spec_precision_t;

typedef enum {
    NPF_FMT_SPEC_LENGTH_MOD_NONE,
    NPF_FMT_SPEC_LENGTH_MOD_SHORT,      /* 'h' */
    NPF_FMT_SPEC_LENGTH_MOD_LONG,       /* 'l' */
    NPF_FMT_SPEC_LENGTH_MOD_LONG_DOUBLE /* 'L' */
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS
    ,
    NPF_FMT_SPEC_LENGTH_MOD_C99_CHAR,      /* 'hh' */
    NPF_FMT_SPEC_LENGTH_MOD_C99_LONG_LONG, /* 'll' */
    NPF_FMT_SPEC_LENGTH_MOD_C99_INTMAX,    /* 'j' */
    NPF_FMT_SPEC_LENGTH_MOD_C99_SIZET,     /* 'z' */
    NPF_FMT_SPEC_LENGTH_MOD_C99_PTRDIFFT   /* 't' */
#endif
} npf__format_spec_length_modifier_t;

typedef enum {
    NPF_FMT_SPEC_CONV_PERCENT,       /* '%' */
    NPF_FMT_SPEC_CONV_CHAR,          /* 'c' */
    NPF_FMT_SPEC_CONV_STRING,        /* 's' */
    NPF_FMT_SPEC_CONV_SIGNED_INT,    /* 'i', 'd' */
    NPF_FMT_SPEC_CONV_OCTAL,         /* 'o' */
    NPF_FMT_SPEC_CONV_HEX_INT,       /* 'x', 'X' */
    NPF_FMT_SPEC_CONV_UNSIGNED_INT,  /* 'u' */
    NPF_FMT_SPEC_CONV_CHARS_WRITTEN, /* 'n' */
    NPF_FMT_SPEC_CONV_POINTER        /* 'p' */
#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS
    ,
    NPF_FMT_SPEC_CONV_FLOAT_DECIMAL,  /* 'f', 'F' */
    NPF_FMT_SPEC_CONV_FLOAT_EXPONENT, /* 'e', 'E' */
    NPF_FMT_SPEC_CONV_FLOAT_DYNAMIC   /* 'g', 'G' */
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS
    ,
    NPF_FMT_SPEC_CONV_C99_FLOAT_HEX /* 'a', 'A' */
#endif
#endif
} npf__format_spec_conversion_t;

typedef enum {
    NPF_FMT_SPEC_CONV_CASE_NONE,
    NPF_FMT_SPEC_CONV_CASE_LOWER,
    NPF_FMT_SPEC_CONV_CASE_UPPER
} npf__format_spec_conversion_case_t;

typedef struct {
    /* optional flags */
    unsigned char left_justified : 1;   /* '-' */
    unsigned char prepend_sign : 1;     /* '+' */
    unsigned char prepend_space : 1;    /* ' ' */
    unsigned char alternative_form : 1; /* '#' */
    unsigned char leading_zero_pad : 1; /* '0' */

    /* field width */
    npf__format_spec_field_width_t field_width_type;
    int field_width;

    /* precision */
    npf__format_spec_precision_t precision_type;
    int precision;

    /* length modifier for specifying argument size */
    npf__format_spec_length_modifier_t length_modifier;

    /* conversion specifiers */
    npf__format_spec_conversion_t conv_spec;
    npf__format_spec_conversion_case_t conv_spec_case;
} npf__format_spec_t;

NPF_INTERFACE_DEF int npf__parse_format_spec(char const *format,
                                             npf__format_spec_t *out_spec);

typedef struct {
    char *dst;
    size_t len;
    size_t cur;
} npf__bufputc_ctx_t;

NPF_INTERFACE_DEF int npf__bufputc(int c, void *ctx);

NPF_INTERFACE_DEF int npf__itoa_rev(char *buf, int i);
NPF_INTERFACE_DEF int npf__utoa_rev(char *buf, unsigned i, int base,
                                    npf__format_spec_conversion_case_t cc);
NPF_INTERFACE_DEF int npf__ptoa_rev(char *buf, void const *p);

#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
NPF_INTERFACE_DEF int npf__fsplit_abs(float f, uint64_t *out_int_part,
                                      uint64_t *out_frac_part);
NPF_INTERFACE_DEF int npf__ftoa_rev(char *buf, float f);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NANOPRINTF_H_INCLUDED */

#else /* NANOPRINTF_IMPLEMENTATION */

#undef NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"
#define NANOPRINTF_IMPLEMENTATION

#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
#include <math.h>
#endif

#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
#include <inttypes.h>
#include <stdint.h>
#include <wchar.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int npf__parse_format_spec(char const *format, npf__format_spec_t *out_spec) {
    char const *cur = format;

    out_spec->left_justified = 0;
    out_spec->prepend_sign = 0;
    out_spec->prepend_space = 0;
    out_spec->alternative_form = 0;
    out_spec->leading_zero_pad = 0;
    out_spec->field_width_type = NPF_FMT_SPEC_FIELD_WIDTH_NONE;
    out_spec->precision_type = NPF_FMT_SPEC_PRECISION_NONE;
    out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_NONE;

    /* Format specifiers start with % */
    if (*cur++ != '%') {
        return 0;
    }

    /* Optional flags */
    while (*cur) {
        switch (*cur++) {
            case '-':
                out_spec->left_justified = 1;
                out_spec->leading_zero_pad = 0;
                continue;
            case '+':
                out_spec->prepend_sign = 1;
                out_spec->prepend_space = 0;
                continue;
            case ' ':
                out_spec->prepend_space = !out_spec->prepend_sign;
                continue;
            case '#':
                out_spec->alternative_form = 1;
                continue;
            case '0':
                out_spec->leading_zero_pad = !out_spec->left_justified;
                continue;
            default:
                break;
        }
        --cur;
        break;
    }

    /* Minimum field width */
    if (*cur == '*') {
        out_spec->field_width_type = NPF_FMT_SPEC_FIELD_WIDTH_STAR;
        ++cur;
    } else {
        out_spec->field_width = 0;
        if (*cur >= '0' && *cur <= '9') {
            out_spec->field_width_type = NPF_FMT_SPEC_FIELD_WIDTH_LITERAL;
        }
        while (*cur >= '0' && *cur <= '9') {
            out_spec->field_width =
                (out_spec->field_width * 10) + (*cur++ - '0');
        }
    }

    /* Precision */
    out_spec->precision = 0;
    if (*cur == '.') {
        ++cur;
        out_spec->precision_type = NPF_FMT_SPEC_PRECISION_LITERAL;
        if (*cur == '*') {
            ++cur;
            out_spec->precision_type = NPF_FMT_SPEC_PRECISION_STAR;
        } else if (*cur == '-') {
            out_spec->precision_type = NPF_FMT_SPEC_PRECISION_NONE;
            ++cur;
            while (*cur >= '0' && *cur <= '9') {
                ++cur;
            }
        } else {
            while (*cur >= '0' && *cur <= '9') {
                out_spec->precision =
                    (out_spec->precision * 10) + (*cur++ - '0');
            }
        }
        out_spec->leading_zero_pad = 0;
    }

    /* Length modifier */
    switch (*cur++) {
        case 'h':
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
            if (*cur == 'h') {
                out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_C99_CHAR;
                ++cur;
            } else
#endif
                out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_SHORT;
            break;
        case 'l':
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
            if (*cur == 'l') {
                out_spec->length_modifier =
                    NPF_FMT_SPEC_LENGTH_MOD_C99_LONG_LONG;
                ++cur;
            } else
#endif
                out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_LONG;
            break;
#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
        case 'L':
            out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_LONG_DOUBLE;
            break;
#endif
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
        case 'j':
            out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_C99_INTMAX;
            break;
        case 'z':
            out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_C99_SIZET;
            break;
        case 't':
            out_spec->length_modifier = NPF_FMT_SPEC_LENGTH_MOD_C99_PTRDIFFT;
            break;
#endif
        default:
            --cur;
            break;
    }

    /* Conversion specifier */
    switch (*cur++) {
        case '%':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_PERCENT;
            break;
        case 'c':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_CHAR;
            break;
        case 's':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_STRING;
            break;
        case 'i':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_SIGNED_INT;
            break;
        case 'd':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_SIGNED_INT;
            break;
        case 'o':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_OCTAL;
            break;
        case 'x':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_HEX_INT;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_LOWER;
            break;
        case 'X':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_HEX_INT;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_UPPER;
            break;
        case 'u':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_UNSIGNED_INT;
            break;
#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
        case 'f':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_DECIMAL;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_LOWER;
            break;
        case 'F':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_DECIMAL;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_UPPER;
            break;
        case 'e':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_EXPONENT;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_LOWER;
            break;
        case 'E':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_EXPONENT;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_UPPER;
            break;
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
        case 'a':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_C99_FLOAT_HEX;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_LOWER;
            break;
        case 'A':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_C99_FLOAT_HEX;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_UPPER;
            break;
#endif
        case 'g':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_DYNAMIC;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_LOWER;
            break;
        case 'G':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_FLOAT_DYNAMIC;
            out_spec->conv_spec_case = NPF_FMT_SPEC_CONV_CASE_UPPER;
            break;
#endif
        case 'n':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_CHARS_WRITTEN;
            break;
        case 'p':
            out_spec->conv_spec = NPF_FMT_SPEC_CONV_POINTER;
            break;
        default:
            return 0;
    }

    return (int)(cur - format);
}

int npf__bufputc(int c, void *ctx) {
    npf__bufputc_ctx_t *bpc = (npf__bufputc_ctx_t *)ctx;
    if (bpc->cur < bpc->len) {
        bpc->dst[bpc->cur++] = (char)c;
        return c;
    }
    return NPF_EOF;
}

int npf__itoa_rev(char *buf, int i) {
    char *dst = buf;
    if (i == 0) {
        *dst++ = '0';
    } else {
        int const neg = (i < 0) ? -1 : 1;
        while (i) {
            *dst++ = (char)('0' + (neg * (i % 10)));
            i /= 10;
        }
    }
    return (int)(dst - buf);
}

int npf__utoa_rev(char *buf, unsigned i, int base,
                  npf__format_spec_conversion_case_t cc) {
    char *dst = buf;
    if (i == 0) {
        *dst++ = '0';
    } else {
        unsigned const base_c =
            (cc == NPF_FMT_SPEC_CONV_CASE_LOWER) ? 'a' : 'A';
        while (i) {
            unsigned const d = i % (unsigned)base;
            i /= (unsigned)base;
            *dst++ = (d < 10) ? (char)('0' + d) : (char)(base_c + (d - 10));
        }
    }
    return (int)(dst - buf);
}

int npf__ptoa_rev(char *buf, void const *p) {
    if (!p) {
        *buf++ = ')';
        *buf++ = 'l';
        *buf++ = 'l';
        *buf++ = 'u';
        *buf++ = 'n';
        *buf++ = '(';
        return 6;
    } else {
        /* c89 requires configuration to learn what uint a void* fits in.
        Instead, just alias to char* and print nibble-by-nibble. */
        unsigned i;
        char const *pb = (char const *)&p;
        char *dst = buf;
        for (i = 0; i < sizeof(void *); ++i) {
            unsigned const d1 = pb[i] & 0xF;
            unsigned const d2 = (pb[i] >> 4) & 0xF;
            *dst++ = (d1 < 10) ? (char)('0' + d1) : (char)('a' + (d1 - 10));
            *dst++ = (d2 < 10) ? (char)('0' + d2) : (char)('a' + (d2 - 10));
        }
        while (*--dst == '0')
            ;
        ++dst;
        *dst++ = 'x';
        *dst++ = '0';
        return (int)(dst - buf);
    }
}

#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
enum {
    NPF_MANTISSA_BITS = 23,
    NPF_EXPONENT_BITS = 8,
    NPF_EXPONENT_BIAS = 127,
    NPF_FRACTION_BIN_DIGITS = 64,
    NPF_MAX_FRACTION_DEC_DIGITS = 8
};

int npf__fsplit_abs(float f, uint64_t *out_int_part, uint64_t *out_frac_part) {
    // conversion algorithm by Wojciech Muła (zdjęcia@garnek.pl)
    // http://0x80.pl/notesen/2015-12-29-float-to-string.html

    // grisu2 (https://bit.ly/2JgMggX) and ryu (https://bit.ly/2RLXSg0)
    // are precise and do rounding, but bigger and require large lookup tables.

    // union-cast is UB, so copy through char*, compiler can optimize.
    uint32_t i_bits;
    {
        char const *src = (char const *)&f;
        char *dst = (char *)&i_bits;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
    }

    // set up the constants
    int const exponent =
        (((i_bits >> NPF_MANTISSA_BITS) & ((1u << NPF_EXPONENT_BITS) - 1)) -
         NPF_EXPONENT_BIAS) -
        NPF_MANTISSA_BITS;
    uint32_t const implicit_one = 1u << NPF_MANTISSA_BITS;
    uint32_t const mantissa = i_bits & (implicit_one - 1);
    uint32_t const mantissa_norm = mantissa | implicit_one;

    // value is out of range
    if (exponent >= (64 - NPF_MANTISSA_BITS)) {
        return 0;
    }

    if (exponent > 0) {
        *out_int_part = mantissa_norm << exponent;
    } else if (exponent < 0) {
        if (-exponent > NPF_MANTISSA_BITS) {
            *out_int_part = 0;
        } else {
            *out_int_part = mantissa_norm >> -exponent;
        }
    } else {
        *out_int_part = mantissa_norm;
    }

    unsigned fraction_dec = 0;
    {
        uint64_t fraction_bin;
        int const shift = NPF_FRACTION_BIN_DIGITS + exponent - 4;
        if ((shift >= (NPF_FRACTION_BIN_DIGITS - 4)) || (shift < 0)) {
            fraction_bin = 0;
        } else {
            fraction_bin = ((uint64_t)mantissa_norm) << shift;
        }

        for (int written = 0; written < NPF_MAX_FRACTION_DEC_DIGITS;
             ++written) {
            fraction_bin &= 0x0fffffffffffffffllu;
            fraction_bin *= 10;
            if (fraction_bin == 0) {
                break;
            }
            fraction_dec *= 10;
            fraction_dec += fraction_bin >> (NPF_FRACTION_BIN_DIGITS - 4);
        }
    }

    *out_frac_part = fraction_dec;
    return 1;
}

int npf__ftoa_rev(char *buf, float f) {
    if (f == 0.0f) {
        *buf++ = '0';
        *buf++ = '0';
        *buf++ = '0';
        *buf++ = '0';
        *buf++ = '0';
        *buf++ = '0';
        *buf++ = '.';
        *buf++ = '0';
        return 8;
    } else if (f != f) {
        *buf++ = 'n';
        *buf++ = 'a';
        *buf++ = 'n';
        return 3;
    } else if (f == INFINITY) {
        *buf++ = 'f';
        *buf++ = 'n';
        *buf++ = 'i';
        return 3;
    }

    uint64_t int_part, frac_part;
    if (npf__fsplit_abs(f, &int_part, &frac_part) == 0) {
        *buf++ = 'r';
        *buf++ = 'o';
        *buf++ = 'o';
        return 3;
    }

    char *dst = buf;
    while (frac_part) {
        *dst++ = '0' + (frac_part % 10);
        frac_part /= 10;
    }
    *dst++ = '.';
    while (int_part) {
        *dst++ = '0' + (int_part % 10);
        int_part /= 10;
    }
    *dst++ = 0;

    return (int)(dst - buf);
}
#endif

int npf_vpprintf(npf_putc pc, void *pc_ctx, char const *format, va_list vlist) {
    npf__format_spec_t fs;
    char const *cur = format;
    int n = 0, sign = 0, i;

#define NPF_PUT_CHECKED(VAL)                \
    do {                                    \
        if (pc((VAL), pc_ctx) == NPF_EOF) { \
            return n;                       \
        }                                   \
        ++n;                                \
    } while (0)

    while (*cur) {
        if (*cur != '%') {
            /* Non-format character, write directly */
            NPF_PUT_CHECKED(*cur++);
        } else {
            /* Might be a format run, try to parse */
            int const fs_len = npf__parse_format_spec(cur, &fs);
            if (fs_len == 0) {
                /* Invalid format specifier, write and continue */
                NPF_PUT_CHECKED(*cur++);
            } else {
                /* Format specifier, convert and write argument */
                char cbuf_mem[24], *cbuf = cbuf_mem, sign_c, pad_c;
                int cbuf_len = 0, pad = 0;

                /* '*' modifiers require more varargs */
                if (fs.field_width_type == NPF_FMT_SPEC_FIELD_WIDTH_STAR) {
                    fs.field_width = va_arg(vlist, int);
                    fs.field_width_type = NPF_FMT_SPEC_FIELD_WIDTH_LITERAL;
                }

                if (fs.precision_type == NPF_FMT_SPEC_PRECISION_STAR) {
                    fs.precision = va_arg(vlist, int);
                    fs.precision_type = NPF_FMT_SPEC_PRECISION_LITERAL;
                }

                /* Convert the argument to string and point cbuf at it */
                switch (fs.conv_spec) {
                    case NPF_FMT_SPEC_CONV_PERCENT:
                        *cbuf = '%';
                        cbuf_len = 1;
                        break;
                    case NPF_FMT_SPEC_CONV_CHAR: /* 'c' */
                        *cbuf = (char)va_arg(vlist, int);
                        cbuf_len = 1;
                        break;
                    case NPF_FMT_SPEC_CONV_STRING: { /* 's' */
                        char *s = va_arg(vlist, char *);
                        cbuf = s;
                        while (*s) ++s;
                        cbuf_len = (int)(s - cbuf);
                    } break;
                    case NPF_FMT_SPEC_CONV_SIGNED_INT: { /* 'i', 'd' */
                        int const val = va_arg(vlist, int);
                        sign = (val < 0) ? -1 : 1;
                        cbuf_len = npf__itoa_rev(cbuf, val);
                    } break;
                    case NPF_FMT_SPEC_CONV_OCTAL: { /* 'o' */
                        unsigned const val = va_arg(vlist, unsigned);
                        cbuf_len =
                            npf__utoa_rev(cbuf, val, 8, fs.conv_spec_case);
                        if (val && fs.alternative_form) {
                            cbuf[cbuf_len++] = '0';
                        }
                    } break;
                    case NPF_FMT_SPEC_CONV_HEX_INT: { /* 'x', 'X' */
                        unsigned const val = va_arg(vlist, unsigned);
                        cbuf_len =
                            npf__utoa_rev(cbuf, val, 16, fs.conv_spec_case);
                        if (val && fs.alternative_form) {
                            cbuf[cbuf_len++] = (fs.conv_spec_case ==
                                                NPF_FMT_SPEC_CONV_CASE_LOWER)
                                                   ? 'x'
                                                   : 'X';
                            cbuf[cbuf_len++] = '0';
                        }
                    } break;
                    case NPF_FMT_SPEC_CONV_UNSIGNED_INT: /* 'u' */
                        cbuf_len = npf__utoa_rev(cbuf, va_arg(vlist, unsigned),
                                                 10, fs.conv_spec_case);
                        break;
                    case NPF_FMT_SPEC_CONV_CHARS_WRITTEN: /* 'n' */
                        cbuf_len = npf__utoa_rev(cbuf, (unsigned)n, 10,
                                                 NPF_FMT_SPEC_CONV_CASE_NONE);
                        break;
                    case NPF_FMT_SPEC_CONV_POINTER: /* 'p' */
                        cbuf_len = npf__ptoa_rev(cbuf, va_arg(vlist, void *));
                        break;
#if NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1
                    case NPF_FMT_SPEC_CONV_FLOAT_DECIMAL: /* 'f', 'F' */
                        break;
                    case NPF_FMT_SPEC_CONV_FLOAT_EXPONENT: /* 'e', 'E' */
                        break;
                    case NPF_FMT_SPEC_CONV_FLOAT_DYNAMIC: /* 'g', 'G' */
                        break;
#if NANOPRINTF_USE_C99_FORMAT_SPECIFIERS == 1
                    case NPF_FMT_SPEC_CONV_C99_FLOAT_HEX: /* 'a', 'A' */
                        break;
#endif
#endif
                }

                /* Compute the leading symbol (+, -, ' ') */
                sign_c = 0;
                if (sign == -1) {
                    sign_c = '-';
                } else if (sign == 1) {
                    if (fs.prepend_sign) {
                        sign_c = '+';
                    } else if (fs.prepend_space) {
                        sign_c = ' ';
                    }
                }

                /* Compute the field width pad character */
                pad_c = 0;
                if (fs.field_width_type == NPF_FMT_SPEC_FIELD_WIDTH_LITERAL) {
                    if (fs.leading_zero_pad) {
                        /* '0' flag is only legal with numeric types */
                        if ((fs.conv_spec != NPF_FMT_SPEC_CONV_STRING) &&
                            (fs.conv_spec != NPF_FMT_SPEC_CONV_CHAR) &&
                            (fs.conv_spec != NPF_FMT_SPEC_CONV_PERCENT)) {
                            pad_c = '0';
                        }
                    } else {
                        pad_c = ' ';
                    }
                }

                if (pad_c) {
                    pad = fs.field_width - cbuf_len - (sign_c ? 1 : 0);
                }

                /* Apply right-justified field width if requested */
                if (!fs.left_justified && pad_c) {
                    /* If leading zeros pad the field, the '-' goes
                     * first. */
                    if (sign_c == '-' && pad_c == '0') {
                        NPF_PUT_CHECKED(sign_c);
                        sign_c = 0;
                    }
                    while (pad-- > 0) {
                        NPF_PUT_CHECKED(pad_c);
                    }
                }

                /* Write the converted payload */
                if (fs.conv_spec == NPF_FMT_SPEC_CONV_STRING) {
                    /* Strings are in the correct order */
                    for (i = 0; i < cbuf_len; ++i) {
                        NPF_PUT_CHECKED(cbuf[i]);
                    }
                } else {
                    if (sign_c) {
                        NPF_PUT_CHECKED(sign_c);
                    }
                    /* *toa leaves numeric payloads reversed */
                    while (cbuf_len--) {
                        NPF_PUT_CHECKED(cbuf[cbuf_len]);
                    }
                }

                /* Apply left-justified field width if requested */
                if (fs.left_justified && pad_c) {
                    while (pad-- > 0) {
                        NPF_PUT_CHECKED(pad_c);
                    }
                }

                cur += fs_len;
            }
        }
    }
    NPF_PUT_CHECKED('\0');
#undef NPF_PUT_CHECKED
    return n;
}

int npf_pprintf(npf_putc pc, void *pc_ctx, char const *format, ...) {
    va_list val;
    int rv;
    va_start(val, format);
    rv = npf_vpprintf(pc, pc_ctx, format, val);
    va_end(val);
    return rv;
}

int npf_snprintf(char *buffer, size_t bufsz, const char *format, ...) {
    va_list val;
    int rv;
    va_start(val, format);
    rv = npf_vsnprintf(buffer, bufsz, format, val);
    va_end(val);
    return rv;
}

int npf_vsnprintf(char *buffer, size_t bufsz, char const *format,
                  va_list vlist) {
    npf__bufputc_ctx_t bufputc_ctx;
    bufputc_ctx.dst = buffer;
    bufputc_ctx.len = bufsz;
    bufputc_ctx.cur = 0;
    return npf_vpprintf(npf__bufputc, &bufputc_ctx, format, vlist);
}

#ifdef __cplusplus
}
#endif

#endif /* NANOPRINTF_IMPLEMENTATION */
