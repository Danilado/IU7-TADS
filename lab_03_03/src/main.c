#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_matrix.h"
#include "my_sparce_matrix.h"
#include "my_utils.h"

#define TEST_COUNT 10

enum MENU_ITEMS
{
    EXIT,
    INPUT_MATRIX_KEYBOARD,
    ADD_ELEMENT,
    REMOVE_ELEMENT,
    INPUT_COLUMN,
    MULTIPLY,
    MULTIPLICATION_TEST,
    PRINT_MATRIX,
    N_MENU_ITEMS
};

int main(void)
{
    int rc = 0;
    int command = 0;

    matrix_t matrix;
    bool matrix_initialized = false;

    sparce_matrix_t sparced;
    bool sparced_initialized = false;

    matrix_t mat_column;
    sparce_matrix_t sparce_column;
    bool column_initialized = false;

    bool running = true;
    while (running)
    {
        printf("Меню:\n"
               "0: Выход\n"
               "1: Ввести матрицу с клавиатуры\n"
               "2: Добавить элемент в матрицу\n"
               "3: Удалить элемент из матрицы\n"
               "4: Ввести столбец для умножения\n"
               "5: Умножить матрицу на столбец\n"
               "6: Сравнить умножение по производительности\n"
               "7: Вывести матрицу\n"
               "> ");

        rc = scanf("%d", &command);
        if (rc != 1)
        {
            clear_stdin();
            command = N_MENU_ITEMS;
            continue;
        }

        switch (command)
        {
        case EXIT:
            running = false;
            break;

        case INPUT_MATRIX_KEYBOARD: {
            size_t rows, columns;
            rc = 0;

            do
            {
                verbose_coord_error(rc);
                rc = input_coords(
                &rows, &columns, 0, 0, "Введите размеры матрицы\n> ");
            } while (rc);

            if (matrix_initialized)
                matrix_destroy(&matrix);
            matrix_initialized = false;

            rc = matrix_init(&matrix, rows, columns);
            if (rc)
            {
                puts("Недостаточно памяти для матрицы");
                break;
            }
            matrix_initialized = true;

            do
            {
                rc = matrix_scanf(&matrix);
            } while (rc);

            if (sparced_initialized)
                sparce_matrix_destroy(&sparced);
            sparced_initialized = false;

            rc = matrix_to_sparced(&matrix, &sparced);
            if (rc)
            {
                puts("Недостаточно памяти для хранения разреженной матрицы");
                break;
            }
            sparced_initialized = true;
        }
        break;

        case ADD_ELEMENT: {
            if (!matrix_initialized)
            {
                puts("Сначала проинициализируйте матрицу (хотя бы введите "
                     "нулевую)");
                break;
            }

            size_t row, column;
            rc = 0;
            do
            {
                verbose_coord_error(rc);
                rc = input_coords(
                &row, &column, 0, 0, "Введите индексы элемента\n> ");
            } while (rc);

            int32_t value;
            do
            {
                if (rc)
                    puts("Ошибка ввода, попробуйте снова");
                puts("Введите значение элемента:");
                rc = scanf("%" SCNd32, &value) != 1;
            } while (rc);

            rc = matrix_add(&matrix, row, column, value);
            if (rc)
            {
                puts("Недостаточно памяти для увеличения матрицы");
            }

            if (sparced_initialized)
                sparce_matrix_destroy(&sparced);
            sparced_initialized = false;

            rc = matrix_to_sparced(&matrix, &sparced);
            if (rc)
            {
                puts("Недостаточно памяти для разреженной матрицы");
                matrix_destroy(&matrix);
                matrix_initialized = false;
                if (column_initialized)
                {
                    matrix_destroy(&mat_column);
                    sparce_matrix_destroy(&sparce_column);
                    column_initialized = false;
                }
                break;
            }
            sparced_initialized = true;

            if (column_initialized)
                if (mat_column.rows != matrix.columns)
                {
                    matrix_destroy(&mat_column);
                    sparce_matrix_destroy(&sparce_column);
                    column_initialized = false;
                }
        }
        break;

        case REMOVE_ELEMENT: {
            if (!matrix_initialized)
            {
                puts("Сначала проинициализируйте матрицу (хотя бы введите "
                     "нулевую)");
                break;
            }

            size_t row, column;
            rc = 0;
            do
            {
                verbose_coord_error(rc);
                rc = input_coords(&row, &column, matrix.rows, matrix.columns,
                "Введите индексы элемента\n> ");
            } while (rc);

            matrix_set(&matrix, row, column, 0);

            if (sparced_initialized)
                sparce_matrix_destroy(&sparced);
            sparced_initialized = false;

            matrix_to_sparced(&matrix, &sparced);
            sparced_initialized = true;
        }
        break;

        case INPUT_COLUMN: {
            if (!matrix_initialized)
            {
                puts("Сначала введите матрицу!");
                break;
            }

            if (column_initialized)
            {
                matrix_destroy(&mat_column);
                sparce_matrix_destroy(&sparce_column);
            }
            column_initialized = false;

            rc = matrix_init(&mat_column, matrix.columns, 1);
            if (rc)
            {
                puts("Не удалось выделить память для матрицы-столбца");
                break;
            }

            matrix_scanf(&mat_column);
            rc = matrix_to_sparced(&mat_column, &sparce_column);
            if (rc)
            {
                puts("Не удалось инициализировать столбец");
                matrix_destroy(&mat_column);
                break;
            }
            column_initialized = true;
        }
        break;

        case MULTIPLY: {
            if (!column_initialized)
            {
                puts("Сначала введите матрицу-столбец для умножения");
                break;
            }

            struct timespec begin, end;
            long time1, time2;
            rc = 0;

            matrix_t res;

            time_now(begin);
            rc = matrix_mul(&matrix, &mat_column, &res);
            time_now(end);

            if (rc)
            {
                puts("Недостаточно памяти для результирующего столбца");
                break;
            }

            time1 = delta_time(begin, end);

            puts("Резльтирующая матрица:");
            matrix_print(&res);
            printf("Время обработки: %ld\n", time1);

            matrix_destroy(&res);

            sparce_matrix_t spar_res;

            rc = sparce_matrix_init(&spar_res);
            if (rc)
                return rc;

            void *tmp = realloc(spar_res.IA, sparced.rows * sizeof(size_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.IA = tmp;

            tmp = realloc(spar_res.A, sparced.rows * sizeof(int32_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.A = tmp;

            tmp = realloc(spar_res.JA, sparced.rows * sizeof(size_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.JA = tmp;

            time_now(begin);
            rc = sparce_matrix_col_mul(&sparced, &sparce_column, &spar_res);
            time_now(end);

            time2 = delta_time(begin, end);

            if (rc)
            {
                puts(
                "Недостаточно памяти для результирующего разреженного столбца");
                break;
            }

            puts("Результирующий столбец:");
            sparce_matrix_print(&spar_res);
            printf("Время обработки: %ld\n", time2);

            printf(
            "\nУмножение разреженной матрицы быстрее в %.6lf раз (%.0lf%%)\n",
            (double)time1 / (double)time2,
            ((double)time1 / (double)time2) * 100);

            sparce_matrix_destroy(&spar_res);
        }
        break;

        case MULTIPLICATION_TEST: {
            if (!column_initialized)
            {
                puts("Сначала введите матрицу-столбец для умножения");
                break;
            }

            struct timespec begin, end;
            long time1, time2;
            unsigned long long sum1, sum2;
            rc = 0;

            for (size_t i = 0; i < TEST_COUNT; ++i)
            {
                printf("Testing matrix %zu/%u      \r", i + 1, TEST_COUNT);
                matrix_t res;
                time_now(begin);
                rc = matrix_mul(&matrix, &mat_column, &res);
                time_now(end);
                if (rc)
                {
                    puts("Ошибка в процессе тестирования");
                    break;
                }
                time1 = delta_time(begin, end);
                sum1 += time1;
                matrix_destroy(&res);
            }
            printf("\n");

            sparce_matrix_t spar_res;
            // ssize_t *row_ends;

            rc = sparce_matrix_init(&spar_res);
            if (rc)
                return rc;

            void *tmp = realloc(spar_res.IA, sparced.rows * sizeof(size_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.IA = tmp;

            tmp = realloc(spar_res.A, sparced.rows * sizeof(int32_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.A = tmp;

            tmp = realloc(spar_res.JA, sparced.rows * sizeof(size_t));
            if (!tmp)
                return EXIT_FAILURE;
            spar_res.JA = tmp;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                printf("Testing sparced %zu/%u     \r", i + 1, TEST_COUNT);
                time_now(begin);
                rc = sparce_matrix_col_mul(&sparced, &sparce_column, &spar_res);
                time_now(end);

                // free(row_ends);
                if (rc)
                {
                    puts("Недостаточно памяти");
                    break;
                }

                time2 = delta_time(begin, end);
                sum2 += time2;
            }
            printf("\n");

            unsigned long long avg1, avg2;
            avg1 = sum1 / TEST_COUNT;
            avg2 = sum2 / TEST_COUNT;

            printf("Анализ скорости умножения:\n");
            printf("Среднее время умножения \"обычной\" матрицы: %lld\n", avg1);
            printf(
            "Среднее время умножения \"разреженной\" матрицы: %lld\n", avg2);
            printf("Прирост производительности: %.0lf%%\n\n",
            ((double)avg1 / (double)avg2) * 100);

            size_t mat_sum_mem =
            sizeof(int32_t) * matrix.rows * matrix.columns +
            sizeof(int32_t) * mat_column.rows;
            size_t spar_sum_mem =
            (sizeof(int32_t) + sizeof(size_t)) * sparced.el_count +
            sizeof(size_t) * sparced.rows +
            (sizeof(int32_t) + sizeof(size_t)) * spar_res.el_count +
            sizeof(size_t) * spar_res.rows;

            sparce_matrix_destroy(&spar_res);

            printf("Анализ использования памяти:\n");
            printf("--- Матрица ---\n");
            printf("Сама матрица: %zu\n",
            sizeof(int32_t) * matrix.rows * matrix.columns);
            printf(
            "Матрица - результат: %zu\n", sizeof(int32_t) * mat_column.rows);
            printf("Суммарная память: %zu\n", mat_sum_mem);
            printf("--- Разреженная Матрица ---\n");
            printf("Исходная матрица: %zu\n",
            (sizeof(int32_t) + sizeof(size_t)) * sparced.el_count +
            sizeof(size_t) * sparced.rows);
            printf("Матрица - результат: %zu\n",
            (sizeof(int32_t) + sizeof(size_t)) * spar_res.el_count +
            sizeof(size_t) * spar_res.rows);
            printf("Суммарная память: %zu\n", spar_sum_mem);
            printf("--- Отношение обычной к разреженной ---\n");
            printf("Обычная занимает в %.6lf раз больше памяти\n\n",
            (double)mat_sum_mem / (double)spar_sum_mem);
        }
        break;

        case PRINT_MATRIX: {
            if (matrix_initialized)
            {
                printf("Матрица в \"обычном виде\":\n");
                matrix_print(&matrix);
            }
            if (sparced_initialized)
            {
                printf("\nМатрица в разреженном виде:\n");
                sparce_matrix_print(&sparced);
            }
            if (column_initialized)
            {
                printf("\nСодержимое матрицы столбца для умножения:\n");
                matrix_print(&mat_column);
                printf("В разреженном виде:\n");
                sparce_matrix_print(&sparce_column);
            }
        }
        break;

        default:
            break;
        }
    }

    if (matrix_initialized)
        matrix_destroy(&matrix);

    if (sparced_initialized)
        sparce_matrix_destroy(&sparced);

    if (column_initialized)
    {
        matrix_destroy(&mat_column);
        sparce_matrix_destroy(&sparce_column);
    }

    return EXIT_SUCCESS;
}
