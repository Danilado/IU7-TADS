#include "my_sparce_matrix.h"

#ifndef NDEBUG
#include <assert.h>
#endif

int sparce_matrix_init(sparce_matrix_t *matrix)
{
    matrix->A = calloc(0, sizeof(int32_t));
    if (!matrix->A)
        return EXIT_FAILURE;
    matrix->JA = calloc(0, sizeof(size_t));
    if (!matrix->JA)
    {
        free(matrix->A);
        return EXIT_FAILURE;
    }
    matrix->IA = calloc(0, sizeof(ssize_t));
    if (!matrix->IA)
    {
        free(matrix->A);
        free(matrix->JA);
        return EXIT_FAILURE;
    }

    matrix->rows = 0;
    matrix->columns = 0;
    matrix->el_count = 0;

    return EXIT_SUCCESS;
}

int sparce_matrix_destroy(sparce_matrix_t *matrix)
{
    free(matrix->A);
    free(matrix->JA);
    free(matrix->IA);

    return EXIT_SUCCESS;
}

int32_t sparce_matrix_get(
const sparce_matrix_t *matrix, size_t row, size_t column)
{
    if (row > matrix->rows)
        return 0;

    if (column > matrix->columns)
        return 0;

    for (ssize_t i = matrix->IA[row]; i < matrix->IA[row + 1]; ++i)
        if (matrix->JA[i] == column)
            return matrix->A[i];

    return 0;
}

void sparce_matrix_print(const sparce_matrix_t *matrix)
{
    printf("Содержимое массива значений (A):\n");
    for (size_t i = 0; i < matrix->el_count; ++i)
        printf("%" PRId32 " ", matrix->A[i]);
    printf("\n");

    printf("Содержимое массива колонок (JA):\n");
    for (size_t i = 0; i < matrix->el_count; ++i)
        printf("%zu ", matrix->JA[i]);
    printf("\n");

    printf("Содержимое массива начал строк (IA):\n");
    for (size_t i = 0; i < matrix->rows; ++i)
        printf("%zd ", matrix->IA[i]);
    printf("\n");
}

int sparce_matrix_col_mul(const sparce_matrix_t *matrix,
const sparce_matrix_t *column, sparce_matrix_t *dst)
{
#ifndef NDEBUG
    assert(column->columns == 1);
    assert(matrix->columns == column->rows);
#endif
    size_t el_count = 0;

    for (size_t i = 0; i < matrix->rows; ++i)
    {
        dst->IA[i] = -1;
        if (matrix->IA[i] < 0)
            continue;

        size_t row_end = matrix->el_count;
        for (size_t j = i + 1; j < matrix->rows; ++j)
            if (matrix->IA[j] > 0)
            {
                row_end = matrix->IA[j];
                break;
            }

        int32_t sum = 0;
        for (size_t k = matrix->IA[i]; k < row_end; ++k)
            sum += matrix->A[k] * column->A[matrix->JA[k]];
        if (sum)
        {
            dst->A[el_count] = sum;
            dst->JA[el_count] = 0;
            dst->IA[i] = el_count;
            ++el_count;
        }
    }

    dst->el_count = el_count;
    dst->columns = 1;
    dst->rows = matrix->rows;

    return EXIT_SUCCESS;
}

// int get_row_ends(const sparce_matrix_t *matrix, ssize_t **row_ends)
// {
//     void *tmp = calloc(matrix->rows, sizeof(size_t));
//     if (!tmp)
//         return EXIT_FAILURE;

//     *row_ends = tmp;

//     for (size_t i = 0; i < matrix->rows; ++i)
//     {
//         if (matrix->IA[i] < 0)
//         {
//             (*row_ends)[i] = -1;
//             continue;
//         }
//         (*row_ends)[i] = matrix->el_count;
//         for (size_t j = i + 1; j < matrix->rows; ++j)
//             if (matrix->IA[j] > 0)
//             {
//                 (*row_ends)[i] = matrix->IA[j];
//                 break;
//             }
//     }

//     return EXIT_SUCCESS;
// }
