#ifndef MY_MATRIX_H
#define MY_MATRIX_H
#define _GNU_SOURCE

#include <inttypes.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

typedef struct
{
    int32_t **rowsptr;
    size_t rows;
    size_t columns;
} matrix_t;

enum matrix_errors
{
    NOT_ENOUGH_MEMORY = 1,
    OUT_OF_BOUNDS,
    SCANF_ERROR,
    SIZE_MISMATCH,
};

int matrix_init(matrix_t *matrix, size_t rows, size_t columns);
int matrix_destroy(matrix_t *matrix);

int matrix_scanf(matrix_t *matrix);
void matrix_print(const matrix_t *matrix);

int32_t matrix_get(const matrix_t *matrix, size_t row, size_t column);
int matrix_set(matrix_t *matrix, size_t row, size_t column, int32_t value);

void matrix_copy(const matrix_t *src, matrix_t *dst);

int matrix_mul(const matrix_t *m1, const matrix_t *m2, matrix_t *dst);
int matrix_add(matrix_t *matrix, size_t row, size_t column, int32_t value);

#endif
