#ifndef BIGNUM_H
#define BIGNUM_H

#define RULER                                                                  \
    "Линейка на 41 символ (со знаком)\n"                \
    "+----1----2----3----4----5----6----7----8\n"
#define PROMPT "Введите множитель (%zu значащих разрядов)\n"

#define MANTISSA_LIMIT 40
#define EXP_ABS_LIMIT 99999

#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXP_CHARACTERS "Ee"

enum errors
{
    MANTISSA_TOO_LONG = 1,
    EXPONENT_TOO_BIG,
    EXPONENT_TOO_SMALL,
    UNSUPPORTED_CHARACTER,
    MISSING_MANTISSA,
    MISSING_SIGN,
    SCAN_ERROR,
};

typedef struct
{
    bool is_negative;
    unsigned char mantissa[MANTISSA_LIMIT];
    int32_t exponent;
} bignum_t;

void shift_mantissa(bignum_t *num, size_t amount);

void bignum_print(const bignum_t *num);
int bignum_scan(bignum_t *dst, size_t max_mantissa, bool silent);

int bignum_to_string(char *dst, const bignum_t *num);
int bignum_sscan(const char *src, bignum_t *num, size_t max_mantissa);

bignum_t double_to_bignum(double num);

int bignum_mul(bignum_t *num1, bignum_t *num2, bignum_t *dst);

void verbose_error(int rc);

#endif