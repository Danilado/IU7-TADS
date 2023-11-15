#include "my_matrix.h"

#ifndef NDEBUG
#include <assert.h>
#endif

int matrix_init(matrix_t *matrix, size_t rows, size_t columns)
{
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->rowsptr = malloc(sizeof(int32_t *) * rows);

    if (!matrix->rowsptr)
        return NOT_ENOUGH_MEMORY;

    for (size_t i = 0; i < matrix->rows; ++i)
    {
        matrix->rowsptr[i] = calloc(columns, sizeof(int32_t));
        if (!matrix->rowsptr[i])
            return NOT_ENOUGH_MEMORY;
    }

    return EXIT_SUCCESS;
}

int matrix_destroy(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; ++i)
        free(matrix->rowsptr[i]);
    free(matrix->rowsptr);

    return EXIT_SUCCESS;
}

int32_t matrix_get(const matrix_t *matrix, size_t row, size_t column)
{
#ifndef NDEBUG
    assert(row < matrix->rows);
    assert(column < matrix->columns);
#endif

    return matrix->rowsptr[row][column];
}

int matrix_set(matrix_t *matrix, size_t row, size_t column, int32_t value)
{
    int rc;
    if (row > matrix->rows || column > matrix->columns)
    {
        matrix_t tmp;
        rc = matrix_init(
        &tmp, max(matrix->rows, row + 1), max(matrix->columns, column + 1));
        if (rc)
            return rc;

        matrix_copy(matrix, &tmp);
        matrix_destroy(matrix);
        *matrix = tmp;
    }

    matrix->rowsptr[row][column] = value;
    return EXIT_SUCCESS;
}

void matrix_copy(const matrix_t *src, matrix_t *dst)
{
    for (size_t i = 0; i < min(src->rows, dst->rows); ++i)
        memcpy(dst->rowsptr[i], src->rowsptr[i],
        sizeof(int32_t) * min(src->columns, dst->columns));
}

int matrix_scanf(matrix_t *matrix)
{
    printf(
    "Введите матрицу (размеры: %zu %zu):\n", matrix->rows, matrix->columns);
    for (size_t i = 0; i < matrix->rows; ++i)
        for (size_t j = 0; j < matrix->columns; ++j)
        {
            int32_t tmp;
            while (scanf("%" SCNd32, &tmp) != 1)
                puts("Ошибка ввода");
            matrix_set(matrix, i, j, tmp);
        }

    return EXIT_SUCCESS;
}

void matrix_print(const matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; ++i)
    {
        for (size_t j = 0; j < matrix->columns; ++j)
            printf("%" PRId32 " ", matrix_get(matrix, i, j));
        printf("\n");
    }
}

int matrix_mul(const matrix_t *m1, const matrix_t *m2, matrix_t *dst)
{
    if (m1->columns != m2->rows)
        return SIZE_MISMATCH;

    int rc;
    rc = matrix_init(dst, m1->rows, m2->columns);
    if (rc)
        return rc;

    for (size_t i = 0; i < m1->rows; ++i)
        for (size_t j = 0; j < m2->columns; ++j)
        {
            int32_t sum = 0;
            for (size_t k = 0; k < m1->columns; ++k)
                sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
            matrix_set(dst, i, j, sum);
        }

    return EXIT_SUCCESS;
}

int matrix_add(matrix_t *matrix, size_t row, size_t column, int32_t value)
{
    if (row >= matrix->rows || column >= matrix->columns)
    {
        int rc;
        matrix_t tmp;
        rc = matrix_init(
        &tmp, max(matrix->rows, (row + 1)), max(matrix->columns, (column + 1)));
        if (rc)
            return rc;

        matrix_copy(matrix, &tmp);
        matrix_destroy(matrix);
        *matrix = tmp;
    }

    matrix_set(matrix, row, column, value);

    return EXIT_SUCCESS;
}
