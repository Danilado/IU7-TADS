#ifndef BIGNUM_H
#define BIGNUM_H

// Смоделировать операцию умножения действительного числа
// на действительное число в форме +-m.n Е +-K, где
// суммарная длина мантиссы первого сомножителя (m+n) - до
// 35 значащих цифр, второго – до 40 значащих цифр, а
// величина порядка K - до 5 цифр. Результат выдать в форме
// +-0.m1 Е +-K1, где m1 – до 40 значащих цифр, а K1 - до 5
// цифр.

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

enum scan_errors
{
    // scan errors
    MANTISSA_TOO_LONG = 1,
    EXPONENT_TOO_BIG,
    EXPONENT_TOO_SMALL,
    UNSUPPORTED_CHARACTER, // in mantissa
    MISSING_MANTISSA,
    MISSING_SIGN,
    SCAN_ERROR,
    // string print errors
    STRING_TOO_SHORT,
};

typedef struct
{
    bool is_negative;
    unsigned char mantissa[MANTISSA_LIMIT];
    int32_t exponent;
} bignum_t;

int get_sign(const char *cptr, bool *dst);
void shift_mantissa(bignum_t *num, size_t amount);
void shift_mantissa_to_end(bignum_t *num, size_t mantissa_len);

void bignum_print(const bignum_t *num);
int bignum_scan(bignum_t *dst, size_t max_mantissa, bool silent);

int bignum_to_string(char *dst, const bignum_t *num);
int bignum_sscan(const char *src, bignum_t *num, size_t max_mantissa);

bignum_t double_to_bignum(double num);
double bignum_to_double(bignum_t num);

int bignum_mul(bignum_t *num1, bignum_t *num2, bignum_t *dst);

#endif