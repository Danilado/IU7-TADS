#include "my_utils.h"

size_t matrix_count_nonzero(const matrix_t *matrix)
{
    size_t res = 0;
    for (size_t i = 0; i < matrix->rows; ++i)
        for (size_t j = 0; j < matrix->columns; ++j)
            if (matrix_get(matrix, i, j))
                ++res;

    return res;
}

long delta_time(struct timespec mt1, struct timespec mt2)
{
    return 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
}

int matrix_to_sparced(const matrix_t *src, sparce_matrix_t *dst)
{
    sparce_matrix_t tmp;
    int rc;
    rc = sparce_matrix_init(&tmp);
    if (rc)
        return rc;

    size_t nonzero_count = 0;
    nonzero_count = matrix_count_nonzero(src);

    void *tmpptr;
    tmpptr = realloc(tmp.A, sizeof(int32_t) * nonzero_count);
    if (!tmpptr)
        return EXIT_FAILURE;
    tmp.A = tmpptr;

    tmpptr = realloc(tmp.JA, sizeof(size_t) * nonzero_count);
    if (!tmpptr)
        return EXIT_FAILURE;
    tmp.JA = tmpptr;

    tmpptr = realloc(tmp.IA, sizeof(ssize_t) * src->rows);
    if (!tmpptr)
        return EXIT_FAILURE;
    tmp.IA = tmpptr;

    size_t newindex = 0;
    for (size_t i = 0; i < src->rows; ++i)
    {
        char empty_row = 1;
        for (size_t j = 0; j < src->columns; ++j)
            if (matrix_get(src, i, j))
            {
                tmp.A[newindex] = matrix_get(src, i, j);
                tmp.JA[newindex] = j;
                if (empty_row)
                {
                    tmp.IA[i] = newindex;
                    empty_row = 0;
                }
                ++newindex;
            }
        if (empty_row)
            tmp.IA[i] = -1;
    }

    tmp.el_count = nonzero_count;
    tmp.rows = src->rows;
    tmp.columns = src->columns;

    *dst = tmp;

    return EXIT_SUCCESS;
}

int sparced_to_matrix(const sparce_matrix_t *src, matrix_t *dst)
{
    matrix_t tmp;
    int rc;
    rc = matrix_init(&tmp, src->rows, src->columns);
    if (rc)
        return rc;

    for (size_t i = 0; i < src->rows; ++i)
    {
        if (src->IA[i] < 0)
            continue;

        size_t IAnext = src->el_count;
        for (size_t j = i + 1; j < src->rows; ++j)
            if (src->IA[j] > 0)
            {
                IAnext = src->IA[j];
                break;
            }

        for (size_t k = src->IA[i]; k < IAnext; ++k)
            matrix_set(&tmp, i, src->JA[k], src->A[k]);
    }

    matrix_destroy(dst);
    *dst = tmp;

    return EXIT_SUCCESS;
}

void clear_stdin(void)
{
    char tmp;
    while ((tmp = getchar()) != '\n' && tmp != EOF)
        ;
}

// 0 limit -> no limit
int input_coords(
size_t *x, size_t *y, size_t xlimit, size_t ylimit, char *prompt)
{
    printf("%s", prompt);
    size_t tmp1, tmp2;
    if (scanf("%zu", &tmp1) != 1)
        return BAD_COORD_INPUT;
    if (scanf("%zu", &tmp2) != 1)
        return BAD_COORD_INPUT;

    if (xlimit)
        if (tmp1 >= xlimit)
            return X_TOO_BIG;
    if (ylimit)
        if (tmp2 >= ylimit)
            return Y_TOO_BIG;

    *x = tmp1;
    *y = tmp2;

    return EXIT_SUCCESS;
}

void verbose_coord_error(int rc)
{
    switch (rc)
    {
    case BAD_COORD_INPUT:
        puts("Ошибка ввода числа!");
        break;

    case X_TOO_BIG:
        puts("Значение x больше предела");
        break;

    case Y_TOO_BIG:
        puts("Значение y больше предела");
        break;

    default:
        break;
    }
}
