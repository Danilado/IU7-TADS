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

enum menu_items
{
    EXIT,
    DO_TEST,
    PUSH_PRINT_SWITCH,
    POP_PRINT_SWITCH,
};

int main(void)
{
    srand(time(NULL) + 42);
    int running = 1;
    while (running)
    {
        printf("Меню:\n"
               "0: Выход\n"
               "1: Провести симуляцию\n"
               "2: Переключить отладку добавки     элементов (сейчас - %s)\n"
               "3: Переключить отладку уничтожения элементов (сейчас - %s)\n",
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

            req_q_destroy(&rq);
        }
        else if (choise == PUSH_PRINT_SWITCH)
        {
            set_verbose_pop(!QUEUE_VERBOSE_POP);
        }
        else if (choise == POP_PRINT_SWITCH)
        {
            set_verbose_pop(!QUEUE_VERBOSE_PUSH);
        }
    }

    return EXIT_SUCCESS;
}
