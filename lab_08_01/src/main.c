#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#define START_HASH_SIZE 25

#include "my_graph.h"

enum menu
{
    EXIT = 0,
    OPEN_FILE,
    READ_FILE,
    WRITE_FILE,
    SHOW_GRAPH,
    ADD_PATH,
    ADD_PATHS,
    ADD_NODE,
    ADD_NODES,
    GET_INACC,
    SHOW_INACC,
};

int graph_add_path(graph_t graph)
{
    if (!graph)
    {
        puts("Сначала инициализируйте граф!");
        return EXIT_FAILURE;
    }

    long from, to;
    printf("Введите два индекса - откуда и куда\n> ");

    if (scanf("%ld%ld", &from, &to) != 2)
    {
        puts("Ошибка чтения!");
        return EXIT_FAILURE;
    }

    int rc = graph_build_path(graph, from, to);
    if (rc)
    {
        if (rc == GRAPH_NO_MEMORY)
            puts("Не хватило памяти...");
        else if (rc == GRAPH_OUT_OF_BOUNDS)
            puts("Индексы вне пределов графа...");
        else
            puts("Возникла непредвиденная ошибка...");
        return rc;
    }

    puts("Дуга успешно добавлена!");
    return EXIT_SUCCESS;
}

int main(void)
{
    graph_t graph = NULL;
    char *filename = NULL;

    int running = 1;

    while (running)
    {
        {
            printf("Текущий файл: %s\n", (filename ? filename : "Не выбран"));
            printf("Меню:\n");
            printf("0: Выход\n");
            printf("1: Выбрать файл для работы\n");
            printf("2: Считать граф из файла\n");
            printf("3: Записать граф в файл\n");
            printf("4: Вывести граф на экран\n");
            printf("5: Добавить дугу в граф\n");
            printf("6: Добавить дуги в граф\n");
            printf("7: Добавить вершину в граф\n");
            printf("8: Добавить вершины в граф\n");
            printf("9: Вывести недоступные из данной вершины вершины\n");
            printf(
            "10: Показать недоступные из данной вершины вершины визуально\n");
            printf(">");
        }

        int tmp;
        if (scanf("%d", &tmp) != 1)
            continue;

        switch (tmp)
        {
        case EXIT: {
            running = 0;
        }
        break;
        case SHOW_GRAPH: {
            if (!graph)
            {
                printf("Сначала инициализируйте граф!\n");
                break;
            }
            if (graph_show(graph))
            {
                printf(
                "Не получилось создать файл с графом - проверьте права\n");
            }
        }
        break;
        case OPEN_FILE: {
            getc(stdin);

            if (filename)
            {
                free(filename);
                filename = NULL;
            }

            if (graph)
                graph_destroy(&graph);

            printf("Введите название файла\n>");
            size_t bufsize = 1;
            char *buf = calloc(bufsize, sizeof(char));
            getline(&buf, &bufsize, stdin);

            buf[strlen(buf) - 1] = '\0';

            FILE *tmp = fopen(buf, "r");
            if (!tmp)
            {
                printf("Файл не открывается или не существует");
                continue;
            }
            fclose(tmp);
            tmp = fopen(buf, "a");
            if (!tmp)
            {
                printf("Нет прав на запись в файл!");
                continue;
            }
            fclose(tmp);

            filename = buf;
        }
        break;
        case READ_FILE: {
            FILE *tmp = fopen(filename, "r");
            if (!tmp)
            {
                printf("Файл не открывается или не существует");
                continue;
            }

            if (graph)
                graph_destroy(&graph);

            graph = graph_create_from_file(tmp);
            if (!graph)
                printf("Возникли ошибки при создании графа (скорее всего не "
                       "хватило памяти)");

            fclose(tmp);
        }
        break;
        case WRITE_FILE: {
            if (!filename)
            {
                printf("Вы не выбрали файл для работы!\n");
                continue;
            }

            if (!graph)
            {
                printf("Сначала инициализируйте граф!\n");
                continue;
            }

            FILE *tmp = fopen(filename, "w");
            if (!tmp)
            {
                puts("Файла не существует, или не хватает прав");
                continue;
            }

            graph_write_to_file(graph, tmp);
            fclose(tmp);
        }
        break;
        case ADD_NODE: {
            if (!filename)
            {
                printf("Вы не выбрали файл для работы!\n");
                continue;
            }

            if (!graph)
            {
                graph = graph_create(1);
                if (!graph)
                {
                    printf("Не удалось инициализировать граф\n");
                    continue;
                }
            }
            else
            {
                if (graph_add_nodes(graph, 1))
                {
                    puts("Возникла ошибка. Скорее всего не хватило памяти");
                    continue;
                };
            }
        }
        break;

        case ADD_NODES: {
            printf("Введите количество узлов: ");
            int tmp;
            if (scanf("%d", &tmp) != 1)
            {
                puts("Ошибка чтения!");
                continue;
            }

            if (!graph)
            {
                graph = graph_create(tmp);
                if (!graph)
                    printf("Не удалось инициализировать граф\n");
                continue;
            }

            if (graph_add_nodes(graph, tmp))
            {
                puts("Возникла ошибка. Скорее всего не хватило памяти");
                continue;
            };
        }
        break;

        case ADD_PATH: {
            if (!graph)
            {
                puts("Сначала инициализируйте граф!");
                break;
            }

            graph_add_path(graph);
        }
        break;

        case ADD_PATHS: {
            if (!graph)
            {
                puts("Сначала инициализируйте граф!");
                break;
            }

            printf("Введите количество дуг: ");
            int tmp;
            if (scanf("%d", &tmp) != 1)
            {
                puts("Ошибка чтения!");
                continue;
            }

            int fails = 0;
            for (int i = 0; i < tmp && fails < 3; ++i)
                if (graph_add_path(graph))
                    ++fails;
        }
        break;

        case GET_INACC: {
            if (!graph)
            {
                puts("Сначала инициализируйте граф!");
                break;
            }

            printf("Введите индекс начального узла: ");
            int tmp;
            if (scanf("%d", &tmp) != 1 || tmp < 0)
            {
                puts("Ошибка чтения!");
                continue;
            }

            int *arr;
            size_t arrlen;

            graph_get_inaccesible_from(graph, tmp, &arr, &arrlen);

            if (!arrlen)
            {
                puts("Все вершины достижимы.");
                continue;
            }

            puts("Недостижимые вершины:");
            for (size_t i = 0; i < arrlen; ++i)
                printf("%d ", arr[i]);
            printf("\n");

            free(arr);
        }
        break;

        case SHOW_INACC: {
            if (!graph)
            {
                puts("Сначала инициализируйте граф!");
                break;
            }

            printf("Введите индекс начального узла: ");
            int tmp;
            if (scanf("%d", &tmp) != 1)
            {
                puts("Ошибка чтения!");
                continue;
            }

            graph_show_w_inaccessible_from(graph, tmp);
        }
        break;

        default:
            break;
        }
    }

    if (filename)
        free(filename);

    if (graph)
        graph_destroy(&graph);

    return EXIT_SUCCESS;
}
