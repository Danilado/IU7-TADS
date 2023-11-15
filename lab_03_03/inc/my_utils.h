#ifndef MY_UTILS_H
#define MY_UTILS_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <sys/time.h>
#include <time.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_matrix.h"
#include "my_sparce_matrix.h"

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)
#define time_now(x) clock_gettime(CLOCK_REALTIME, &x);

int matrix_to_sparced(const matrix_t *src, sparce_matrix_t *dst);
int sparced_to_matrix(const sparce_matrix_t *src, matrix_t *dst);

long delta_time(struct timespec mt1, struct timespec mt2);

enum INPUT_ERRORS
{
    BAD_COORD_INPUT = 1,
    X_TOO_BIG,
    Y_TOO_BIG,
};
int input_coords(
size_t *x, size_t *y, size_t xlimit, size_t ylimit, char *prompt);
void verbose_coord_error(int rc);
void clear_stdin(void);

#endif
