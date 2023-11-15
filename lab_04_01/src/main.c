#include "my_arr_stack.h"
#include "my_l_stack.h"
#include "my_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT 1000

enum menu_items
{
    EXIT,
    PRINT,
    PRINT_ADRESSES,
    PUSH,
    POP,
    PRINT_SERIES,
    PROFILING,
};

void print_list_node_adress(my_list_node_t *node)
{
    printf("%p\n", (void *)node);
}

int main(int argc, char **argv)
{
    int rc;

    dynarr_stack_t arr_stack;
    list_stack_t l_stack;

    rc = list_stack_init(&l_stack);
    if (rc)
        return rc;

    arr_stack_init(&arr_stack);

    // считывание из файла
    if (argc > 1)
    {
        FILE *fin = fopen(argv[1], "r");
        if (!fin)
        {
            puts("Переданный файл не открывается");
            return EXIT_FAILURE;
        }

        printf("Считываем: ");
        int tmp;
        while (fscanf(fin, "%d", &tmp) == 1 && arr_stack.arr.el_count <= 10000)
        {
            rc = list_stack_push(&l_stack, tmp);
            if (rc)
            {
                fclose(fin);
                return 10 + rc;
            }

            rc = arr_stack_push(&arr_stack, tmp);
            if (rc)
            {
                fclose(fin);
                return 20 + rc;
            }

            printf("%d ", tmp);
        }
        printf("\n");

        printf("Из файла %s считано %zu элементов\n", argv[1],
        (size_t)(arr_stack.PS - arr_stack.arr.pb) + 1);
        fclose(fin);
    }

    bool running = true;
    while (running)
    {
        int command;

        printf("Меню:\n"
               "0: Выход\n"
               "1: Вывести стеки\n"
               "2: Вывести адреса элементов стека через список\n"
               "3: Добавить элемент в стек\n"
               "4: Удалить последний введённый элемент\n"
               "5: Вывести серии\n"
               "6: Провести замеры\n"
               ">");

        if (scanf("%d", &command) != 1)
        {
            clear_stdin();
            continue;
        }

        switch (command)
        {
        case EXIT:
            running = false;
            break;

        case PRINT: {
            printf("Стек через список:\n");
            list_print(&l_stack.list);
            printf("\n\n");

            printf("Стек через массив:\n");
            arr_print(&arr_stack.arr);
            printf("\n\n");
        }
        break;

        case PRINT_ADRESSES: {
            printf("Адреса элементов стека через список:\n");
            list_apply(&l_stack.list, print_list_node_adress);
            printf("\n");
        }
        break;

        case PUSH: {
            int tmp;
            do
            {
                printf("Введите число для добавления\n>");
            } while (scanf("%d", &tmp) != 1);

            rc = list_stack_push(&l_stack, tmp);
            if (rc)
            {
                printf(
                "Возникла ошибка при добавлении элемента в стек списком");
                continue;
            }
            rc = arr_stack_push(&arr_stack, tmp);
            if (rc)
            {
                printf(
                "Возникла ошибка при добавлении элемента в стек массивом");
                continue;
            }
        }
        break;

        case POP: {
            if (!l_stack.list.head)
            {
                puts("Стеки и так пусты\n");
                continue;
            }

            int buf;
            rc = arr_stack_pop(&arr_stack, &buf);
            rc = list_stack_pop(&l_stack, &buf);

            if (rc)
                printf("Стеки пусты\n\n");
            else
                printf("Возвращено число: %d\n\n", buf);
        }
        break;

        case PRINT_SERIES: {
            dynarr_stack_t da_stack_copy;
            if (rc)
            {
                puts("Недостаточно памяти для создания копии стека");
                continue;
            }
            da_stack_copy.PS =
            da_stack_copy.arr.pb + (arr_stack.PS - arr_stack.arr.pb);

            memcpy(da_stack_copy.arr.pb, arr_stack.arr.pb,
            (arr_stack.arr.el_count) * sizeof(int));

            arr_stack_print_desc_series_backwards(&da_stack_copy, stdout);
            printf("\n\n");

            list_stack_t l_stack_copy;
            rc = list_stack_init(&l_stack_copy);
            if (rc)
            {
                puts("Недостаточно памяти для создания копии стека");
                continue;
            }

            rc = list_copy(&l_stack.list, &l_stack_copy.list);
            if (rc)
            {
                list_stack_destroy(&l_stack_copy, NULL);
                puts("Недостаточно памяти для создания копии стека");
                continue;
            }

            list_stack_print_desc_series_backwards(&l_stack_copy, stdout);
            printf("\n\n");

            list_stack_destroy(&l_stack_copy, NULL);
        }
        break;

        case PROFILING: {
            if (!l_stack.list.head)
            {
                puts("Стеки и пусты\n");
                continue;
            }

            struct timespec begin, end;
            unsigned long long sum1 = 0, sum2 = 0;

            for (size_t i = 0; i < TEST_COUNT; ++i)
            {
                dynarr_stack_t da_stack_copy;
                if (rc)
                {
                    puts("Недостаточно памяти для создания копии стека");
                    continue;
                }
                da_stack_copy.PS =
                da_stack_copy.arr.pb + (arr_stack.PS - arr_stack.arr.pb);

                memcpy(da_stack_copy.arr.pb, arr_stack.arr.pb,
                (arr_stack.arr.el_count) * sizeof(int));

                time_now(begin);
                arr_stack_print_desc_series_backwards(&da_stack_copy, NULL);
                time_now(end);

                sum1 += delta_time(begin, end);
            }

            for (size_t i = 0; i < TEST_COUNT; ++i)
            {
                list_stack_t l_stack_copy;
                rc = list_stack_init(&l_stack_copy);
                if (rc)
                {
                    puts("Недостаточно памяти для создания копии стека");
                    continue;
                }

                rc = list_copy(&l_stack.list, &l_stack_copy.list);
                if (rc)
                {
                    list_stack_destroy(&l_stack_copy, NULL);
                    puts("Недостаточно памяти для создания копии стека");
                    continue;
                }

                time_now(begin);
                list_stack_print_desc_series_backwards(&l_stack_copy, NULL);
                time_now(end);

                sum2 += delta_time(begin, end);

                list_stack_destroy(&l_stack_copy, NULL);
            }

            printf("Сравнение по времени обработки стеков (в наносекундах):\n");
            printf(
            "Среднее время обработки стека через статический массив: %llu\n",
            sum1 / TEST_COUNT);
            printf(
            "Среднее время обработки стека через список:             %llu\n",
            sum2 / TEST_COUNT);
            printf("\n");
            printf("Размеры стеков (в Байтах):\n");
            printf("Размер стека через массив: %zu\n", sizeof(arr_stack));
            printf("Размер стека через список: %zu\n",
            sizeof(my_list_node_t) * (list_len(&l_stack.list) + 1));
            printf("\n");
            printf("Сравнительные характеристики:\n");
            printf("Стек через массив обрабатывается за %.0lf%% от времени "
                   "обработки стека через список\n",
            (double)sum1 / (double)sum2 * 100.0);
            printf("Стек через массив занимает %.0lf%% от размера стека через "
                   "список\n\n",
            (double)sizeof(arr_stack) /
            (double)(sizeof(my_list_node_t) * (list_len(&l_stack.list) + 1)) *
            100.0);
        }
        break;

        default:
            continue;
            break;
        }
    }

    size_t llen = list_len(&l_stack.list) + 1;
    if (llen)
    {
        my_list_node_t *graveyard[llen];
        list_stack_destroy(&l_stack, graveyard);
        printf("Адреса очищенных элементов:\n");

        for (size_t i = 0; i < llen; ++i)
            printf("%zu: %p\n", i, (void *)graveyard[i]);

        printf("\nФрагментированные индексы:\n");

        for (size_t i = 0; i < llen - 1; ++i)
            if (labs(graveyard[i] - graveyard[i + 1]) > sizeof(my_list_node_t))
                printf("Индексы %.5zu %.5zu Адреса: %p %p\n", i, i + 1,
                (void *)graveyard[i], (void *)graveyard[i + 1]);
    }
    else
        list_stack_destroy(&l_stack, NULL);

    return EXIT_SUCCESS;
}