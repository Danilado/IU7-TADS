#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef POSIX_C_SOURCE
#define POSIX_C_SOURCE 200809L
#endif

#define TARGET_QUEUE_LENGTH 1000

#include "my_req_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
                _                       __
              /   \                  /      \
             '      \              /          \
            |       |Oo          o|            |
            `    \  |OOOo......oOO|   /        |
             `    \\OOOOOOOOOOOOOOO\//        /
               \ _o\OOOOOOOOOOOOOOOO//. ___ /
           ______OOOOOOOOOOOOOOOOOOOOOOOo.___
            --- OO'* `OOOOOOOOOO'*  `OOOOO--
                OO.   OOOOOOOOO'    .OOOOO o
                `OOOooOOOOOOOOOooooOOOOOO'OOOo
              .OO "OOOOOOOOOOOOOOOOOOOO"OOOOOOOo
          __ OOOOOO`OOOOOOOOOOOOOOOO"OOOOOOOOOOOOo
         ___OOOOOOOO_"OOOOOOOOOOO"_OOOOOOOOOOOOOOOO
           OOOOO^OOOO0`(____)/"OOOOOOOOOOOOO^OOOOOO
           OOOOO OO000/000000\000000OOOOOOOO OOOOOO
           OOOOO O0000000000000000 ppppoooooOOOOOO
           `OOOOO 0000000000000000 QQQQ "OOOOOOO"
            o"OOOO 000000000000000oooooOOoooooooO'
            OOo"OOOO.00000000000000000000OOOOOOOO'
           OOOOOO QQQQ 0000000000000000000OOOOOOO
          OOOOOO00eeee00000000000000000000OOOOOOOO.
         OOOOOOOO000000000000000000000000OOOOOOOOOO
         OOOOOOOOO00000000000000000000000OOOOOOOOOO
         `OOOOOOOOO000000000000000000000OOOOOOOOOOO
           "OOOOOOOO0000000000000000000OOOOOOOOOOO'
             "OOOOOOO00000000000000000OOOOOOOOOO"
  .ooooOOOOOOOo"OOOOOOO000000000000OOOOOOOOOOO"
.OOO"""""""""".oOOOOOOOOOOOOOOOOOOOOOOOOOOOOo
OOO         QQQQO"'                     `"QQQQ
OOO
`OOo.
  `"OOOOOOOOOOOOoooooooo.
*/

enum menu_items
{
    EXIT,
    DO_TEST,
    PUSH_PRINT_SWITCH,
    POP_PRINT_SWITCH,
};

int main(void)
{
    int QUEUE_VERBOSE_PUSH = 0;
    int QUEUE_VERBOSE_POP = 0;

    srand(time(NULL) + 42);
    int running = 1;
    while (running)
    {
        printf("Меню:\n"
               "0: Выход\n"
               "1: Провести симуляцию\n"
               "2: Переключить отладку добавки     элементов (сейчас - %s)\n"
               "3: Переключить отладку уничтожения элементов (сейчас - %s)\n"
               "> ",
        (QUEUE_VERBOSE_PUSH) ? "вкл" : "выкл",
        (QUEUE_VERBOSE_POP) ? "вкл" : "выкл");

        int choise;
        if (scanf("%d", &choise) != 1)
            continue;

        if (choise == EXIT)
        {
            running = 0;
        }
        else if (choise == DO_TEST)
        {
            req_queue_t rq = req_q_create();
            if (!rq)
            {
                puts("Не удалось инициализировать очередь");
                continue;
            }

            size_t full_q_len = req_q_fill(rq, TARGET_QUEUE_LENGTH);
            if (full_q_len != TARGET_QUEUE_LENGTH)
            {
                puts("Не удалось заполнить очередь (нехватка памяти)");
                req_q_destroy(&rq);
                continue;
            }

            size_t proc_count = 0;
            size_t t1_count = 0;

            size_t t2_prev = 0;

            while (t1_count < TARGET_QUEUE_LENGTH)
            {
                enum req_type t = req_q_process_next(rq);

                ++proc_count;
                if (t == TYPE_ONE)
                    ++t1_count;

                if (t == TYPE_ONE && t1_count % 100 == 0)
                {
                    printf("Обработано %zu заявок первого типа\n", t1_count);
                    printf(
                    "Текущая длина очереди: %zu\n", req_q_get_cur_length(rq));
                    printf("Средняя длина очереди: %.2lf\n",
                    req_q_get_cur_avg_len(rq));
                    printf("Среднее время в очереди: %.2lf\n",
                    req_q_get_cur_avg_wait(rq));
                    printf("Всего заявок обработано: %zu\n",
                    t1_count + req_q_get_t2_out(rq));
                    printf("Только за последний промежуток: %zu\n\n",
                    100 + req_q_get_t2_out(rq) - t2_prev);
                    t2_prev = +req_q_get_t2_out(rq);
                }
            }

            printf("Обработка окончена\n");
            printf(
            "Общее время моделирования: %.6lf\n", req_q_get_cur_time(rq));
            printf("Время простоя Аппарата: %.6lf\n", req_q_get_sleep_time(rq));
            printf(
            "Вошло и вышло заявок первого типа: %zu\n", req_q_get_t1_out(rq));
            printf(
            "Вошло и вышло заявок второго типа: %zu\n", req_q_get_t2_out(rq));

            req_q_destroy(&rq);
        }
        else if (choise == PUSH_PRINT_SWITCH)
        {
            set_verbose_push(!QUEUE_VERBOSE_PUSH);
            QUEUE_VERBOSE_PUSH = !QUEUE_VERBOSE_PUSH;
        }
        else if (choise == POP_PRINT_SWITCH)
        {
            set_verbose_pop(!QUEUE_VERBOSE_POP);
            QUEUE_VERBOSE_POP = !QUEUE_VERBOSE_POP;
        }
    }

    return EXIT_SUCCESS;
}
