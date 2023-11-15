#ifndef MY_SPARCE_MATRIX_H
#define MY_SPARCE_MATRIX_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>

#include <unistd.h>

#include <inttypes.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

typedef struct
{
    size_t rows;
    size_t columns;
    size_t el_count;

    int32_t *A;
    size_t *JA;
    ssize_t *IA;
} sparce_matrix_t;

int sparce_matrix_init(sparce_matrix_t *matrix);
int sparce_matrix_destroy(sparce_matrix_t *matrix);

int32_t sparce_matrix_get(
const sparce_matrix_t *matrix, size_t row, size_t column);

void sparce_matrix_print(const sparce_matrix_t *matrix);
int sparce_matrix_col_mul(const sparce_matrix_t *matrix,
const sparce_matrix_t *column, sparce_matrix_t *dst);
// int get_row_ends(const sparce_matrix_t *matrix, ssize_t **row_ends);

#endif
