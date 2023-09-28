#define _POSIX_C_SOURCE 199309L

#include "record.h"
#include "record_table.h"
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_RUN_COUNT 1000

enum modes
{
    EXIT,
    PRINT_TABLE,
    ADD_EL,
    SORT_TABLE,
    FILTER_TABLE,
    DELETE_EL,
    TIME_TABLE,
    N_MODES,
};

long delta_time(struct timespec mt1, struct timespec mt2)
{
    return 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
}

int main(int argc, char **argv)
{
    FILE *db_file;
    bool writeonly = false;

    if (argc < 2)
    {
        puts("Недостаточно аргументов\nВызов приложения:\n$ app.exe "
             "<filename>\nfilename - имя файла БД");
        return -1;
    }

    db_file = fopen(argv[1], "rb+");
    if (db_file == NULL)
    {
        puts("Не удалось открыть файл.\nВозможно его не "
             "существует.\nПопробовать создать?\n[Y/n] > ");

        char tmp;
        tmp = getchar();
        if (tmp == 'n' || tmp == 'N')
            return -1;
        if (tmp != '\n' && tmp != EOF)
            clear_stdin();

        writeonly = true;
        db_file = fopen(argv[1], "wb");
        if (db_file == NULL)
        {
            puts("Не удалось создать файл");
            return -1;
        }
    }
    fclose(db_file);

    record_table_t table;

    int rc, mode;

    if (!writeonly)
        rc = table_fscan(&table, argv[1]);
    else
        rc = rec_table_init(&table, 5);

    if (rc)
    {
        if (rc == NOT_ENOUGH_MEMORY)
            puts("Недостаточно памяти для хранения таблицы");
        if (rc == BAD_FILE_SIZE)
            puts("Кажется, файл повреждён");
        return rc;
    }

    do
    {
        puts("Введите режим работы:\n"
             "1: Вывести таблицу\n"
             "2: Добавить элемент в таблицу\n"
             "3: Отсортировать таблицу\n"
             "4: Вывести проходящие по фильтру (из задания) записи\n"
             "5: Удалить элемент из таблицы\n"
             "6: Вывести таблицу эффективности сортировки\n"
             "0: Выход");
        printf("> ");

        rc = scanf("%d", &mode);
        if (rc != 1)
        {
            clear_stdin();
            mode = N_MODES;
            continue;
        }

        getchar();

        switch (mode)
        {
        case PRINT_TABLE: {
            rec_table_print(&table);
        }
        break;
        case ADD_EL: {
            record_t tmp;
            record_scan(&tmp);
            rc = rec_table_push(&table, &tmp);
            if (rc)
            {
                printf("На сохранение таблицы не хватило памяти");
                return NOT_ENOUGH_MEMORY;
            }
            rc = table_fwrite(&table, argv[1]);
            if (rc)
            {
                printf("Ошибка записи в файл");
                return CANT_WRITE;
            }
        }
        break;
        case SORT_TABLE: {
            int tmp = 0;
            do
            {
                printf("Введите поле для сортировки\n"
                       "1: Название театра\n"
                       "2: Минимальная цена билета\n"
                       "> ");
                rc = scanf("%d", &tmp);
                if (rc != 1)
                {
                    clear_stdin();
                    tmp = 0;
                    continue;
                }

                getchar();
            } while (!tmp);

            struct timespec begin, end;
            long time1, time2;
            record_table_t tmptable;
            key_t *key_table = NULL;

            rc = copy_table(&table, &tmptable);
            if (rc)
            {
                puts("Недостаточно памяти для создания копии таблицы");
                return rc;
            }

            rc = form_key_table(&table, &key_table, tmp);
            if (rc)
            {
                puts("Недостаточно памяти для таблицы ключей");
                return rc;
            }

            clock_gettime(CLOCK_REALTIME, &begin);

            sort_key_table(&table, key_table, tmp);

            clock_gettime(CLOCK_REALTIME, &end);
            time2 = delta_time(begin, end);

            clock_gettime(CLOCK_REALTIME, &begin);

            sort_table(&tmptable, tmp);

            clock_gettime(CLOCK_REALTIME, &end);
            time1 = delta_time(begin, end);

            puts("Таблица после сортировки");
            rec_table_print(&tmptable);
            puts("");

            puts("Таблица после сортировки с помощью таблицы ключей");
            rec_table_print_keyt(&table, key_table);
            puts("");

            puts("Затраты времени:");
            printf("Сортировка таблицы        : %ld\n", time1);
            printf("Сортировка таблицы ключей : %ld\n", time2);
            printf("Сортировка таблицы ключей быстрее в %.3lf раз\n",
            (double)time1 / (double)time2);
            free(key_table);
        }
        break;
        case FILTER_TABLE: {
            size_t count = 0;
            bool flags[table.el_count];
            int32_t age = 0, length = 0;

            do
            {
                printf("Введите возраст зрителя (>=3)\n"
                       "> ");
                rc = scanf("%d", &age);
                if (rc != 1)
                {
                    clear_stdin();
                    age = 0;
                    continue;
                }

                if (age < 3)
                    age = 0;

                getchar();
            } while (!age);

            do
            {
                printf("Введите максимальную длину выступления в минутах (>0)\n"
                       "> ");
                rc = scanf("%d", &length);
                if (rc != 1)
                {
                    clear_stdin();
                    length = 0;
                    continue;
                }

                if (length < 0)
                    length = 0;

                getchar();
            } while (!length);

            count = filter_table(&table, flags, age, length);
            if (!count)
                puts("Подходящих записей не найдено");

            rec_table_print_flagged(&table, flags);
        }
        break;
        case DELETE_EL: {
            size_t index;
            do
            {
                printf("Введите номер элемента для удаления (не индекс)\n"
                       "(сейчас в таблице %zu записей)> ",
                table.el_count);
                rc = scanf("%zu", &index);
                if (rc != 1)
                {
                    clear_stdin();
                    index = 0;
                    continue;
                }

                if (index > table.el_count)
                    index = 0;

                getchar();
            } while (!index);
            --index;
            rec_table_del(&table, index);
            rc = table_fwrite(&table, argv[1]);
            if (rc)
            {
                printf("Ошибка удаления");
                return rc;
            }
        }
        break;
        // case TIME_TABLE: {
        //     double avg_time1 = 0, avg_time2 = 0;

        //     struct timespec begin, end;
        //     long time1, time2;
        //     record_table_t tmptable;
        //     key_t *key_table = NULL;

        //     rc = copy_table(&table, &tmptable);
        //     if (rc)
        //     {
        //         puts("Недостаточно памяти для создания копии таблицы");
        //         return rc;
        //     }

        //     rc = form_key_table(&table, &key_table, 1);
        //     // rc = form_key_table(&table, &key_table, 2);
        //     if (rc)
        //     {
        //         puts("Недостаточно памяти для таблицы ключей");
        //         return rc;
        //     }

        //     for (size_t i = 0; i < TEST_RUN_COUNT; i++)
        //     {
        //         clock_gettime(CLOCK_REALTIME, &begin);

        //         sort_key_table(&table, key_table, 1);

        //         clock_gettime(CLOCK_REALTIME, &end);
        //         time2 = delta_time(begin, end);
        //         if (avg_time2 < 1E-8)
        //             avg_time2 = time2;
        //         else
        //         {
        //             avg_time2 += time2;
        //             avg_time2 /= 2.0;
        //         }
        //     }
        // }
        default:
            break;
        }

    } while (mode != EXIT);

    return EXIT_SUCCESS;
}
