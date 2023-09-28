#include "record_table.h"

// "┌────────┬──────────────────────┬──────────────────────┬───────────────────┬───────────────┬────────────┬──────────────────────┬──────────────────────┬─────────┬──────────────────┐"
// "│──#NUM──│───НАЗВАНИЕ──ТЕАТРА───│──НАЗВАНИЕ─СПЕКТАКЛЯ──│─ДИАПАЗ.─БИЛ─(руб)─│─ТИП─СПЕКТАКЛЯ─│─ТИП─МУЗЫК.─│──────КОМПОЗИТОР──────│────────СТРАНА────────│─ВОЗРАСТ─│─Продолжит.─(мин)─|"
// "├────────┼──────────────────────┼──────────────────────┼───────────────────┼───────────────┼────────────┼──────────────────────┼──────────────────────┼─────────┼──────────────────┤"
// "└────────┴──────────────────────┴──────────────────────┴───────────────────┴───────────────┴────────────┴──────────────────────┴──────────────────────┴─────────┴──────────────────┘"

void rec_table_print(const record_table_t *table)
{
    if (table->el_count == 0)
    {
        puts("Таблица пуста\n");
        return;
    }

    puts(
    "┌────────┬──────────────────────┬──────────────────────┬───────────────"
    "────┬─"
    "──────────────┬────────────┬──────────────────────┬──────────────────"
    "────┬─────────┬──────────────────┐");
    puts("│  #NUM  │   НАЗВАНИЕ  ТЕАТРА   │  НАЗВАНИЕ СПЕКТАКЛЯ  │ ДИАПАЗ. БИЛ "
         "(руб) │ "
         "ТИП СПЕКТАКЛЯ │ ТИП МУЗЫК. │      КОМПОЗИТОР      │        СТРАНА    "
         "    │ ВОЗРАСТ │ Продолжит. (мин) │");
    puts(
    "├────────┼──────────────────────┼──────────────────────┼───────────────"
    "────┼─"
    "──────────────┼────────────┼──────────────────────┼──────────────────"
    "────┼─────────┼──────────────────┤");

    for (size_t i = 0; i < table->el_count; ++i)
        record_print(&table->dataptr[i], i + 1);

    puts(
    "└────────┴──────────────────────┴──────────────────────┴───────────────"
    "────┴─"
    "──────────────┴────────────┴──────────────────────┴──────────────────"
    "────┴─────────┴──────────────────┘");
}

void rec_table_print_keyt(const record_table_t *table, key_t *order)
{
    if (table->el_count == 0)
    {
        puts("Таблица пуста\n");
        return;
    }

    puts(
    "                "
    "┌────────┬──────────────────────┬──────────────────────┬───────────────"
    "────┬─"
    "──────────────┬────────────┬──────────────────────┬──────────────────"
    "────┬─────────┬──────────────────┐");
    puts("INDEX  MAIN_IND │  #NUM  │   НАЗВАНИЕ  ТЕАТРА   │  НАЗВАНИЕ "
         "СПЕКТАКЛЯ  │ ДИАПАЗ. БИЛ "
         "(руб) │ "
         "ТИП СПЕКТАКЛЯ │ ТИП МУЗЫК. │      КОМПОЗИТОР      │        СТРАНА    "
         "    │ ВОЗРАСТ │ Продолжит. (мин) │");
    puts(
    "                "
    "├────────┼──────────────────────┼──────────────────────┼───────────────"
    "────┼─"
    "──────────────┼────────────┼──────────────────────┼──────────────────"
    "────┼─────────┼──────────────────┤");

    for (size_t i = 0; i < table->el_count; ++i)
    {
        printf("%6zu   %6zu ", i + 1, order[i].main_index + 1);
        record_print(&table->dataptr[order[i].main_index], i + 1);
    }

    puts(
    "                "
    "└────────┴──────────────────────┴──────────────────────┴───────────────"
    "────┴─"
    "──────────────┴────────────┴──────────────────────┴──────────────────"
    "────┴─────────┴──────────────────┘");
}

void rec_table_print_flagged(const record_table_t *table, bool *flags)
{
    if (table->el_count == 0)
    {
        puts("Таблица пуста\n");
        return;
    }

    puts(
    "┌────────┬──────────────────────┬──────────────────────┬───────────────"
    "────┬─"
    "──────────────┬────────────┬──────────────────────┬──────────────────"
    "────┬─────────┬──────────────────┐");
    puts("│  #NUM  │   НАЗВАНИЕ  ТЕАТРА   │  НАЗВАНИЕ СПЕКТАКЛЯ  │ ДИАПАЗ. БИЛ "
         "(руб) │ "
         "ТИП СПЕКТАКЛЯ │ ТИП МУЗЫК. │      КОМПОЗИТОР      │        СТРАНА    "
         "    │ ВОЗРАСТ │ Продолжит. (мин) │");
    puts(
    "├────────┼──────────────────────┼──────────────────────┼───────────────"
    "────┼─"
    "──────────────┼────────────┼──────────────────────┼──────────────────"
    "────┼─────────┼──────────────────┤");

    for (size_t i = 0; i < table->el_count; ++i)
        if (flags[i])
            record_print(&table->dataptr[i], i + 1);

    puts(
    "└────────┴──────────────────────┴──────────────────────┴───────────────"
    "────┴─"
    "──────────────┴────────────┴──────────────────────┴──────────────────"
    "────┴─────────┴──────────────────┘");
}

int rec_table_init(record_table_t *table, size_t el_count)
{
    table->el_count = 0;
    table->dataptr = calloc(el_count, sizeof(record_t));
    if (!table->dataptr)
        return NOT_ENOUGH_MEMORY;
    table->reserved_el_count = el_count;
    return EXIT_SUCCESS;
}

int rec_table_push(record_table_t *table, record_t *record)
{
    if (table->el_count >= table->reserved_el_count)
    {
        void *tmp =
        calloc((size_t)(table->reserved_el_count * 1.15 + 1), sizeof(record_t));
        if (!tmp)
            return NOT_ENOUGH_MEMORY;

        memcpy(
        tmp, table->dataptr, table->reserved_el_count * sizeof(record_t));

        free(table->dataptr);
        table->dataptr = tmp;

        table->reserved_el_count =
        (size_t)(table->reserved_el_count * 1.15 + 1);
    }

    table->dataptr[table->el_count] = *record;
    ++table->el_count;

    return EXIT_SUCCESS;
}

int table_fscan(record_table_t *table, char *filename)
{
    size_t el_count = 0, filesize;
    int rc;
    FILE *f = fopen(filename, "rb");

    fseek(f, 0, SEEK_END);

    filesize = ftell(f);

    if (filesize % sizeof(record_t))
        return BAD_FILE_SIZE;

    el_count = filesize / sizeof(record_t);

    if (el_count == 0)
    {
        rec_table_init(table, 5);
        return EXIT_SUCCESS;
    }

    rc = rec_table_init(table, el_count);
    if (rc)
        return rc;

    fseek(f, 0, SEEK_SET);

    fread(table->dataptr, sizeof(record_t), el_count, f);
    table->el_count = el_count;
    table->reserved_el_count = el_count;

    fclose(f);

    rc = validate_table(table, filename);
    if (rc)
        return rc;

    return EXIT_SUCCESS;
}

void verbose_record_error(int rc)
{
    switch (rc)
    {
    case STRING_FIELDS_OVERLAP:
        puts("У какой-то из строк в таблице нет терминирующего нуля");
        break;
    case NO_STRING:
        puts("Одна из строк в таблице пустая");
        break;
    case PLAY_TYPE_OUT_OF_BOUNDS:
        puts("Ошибка тип спектакля по данному номеру не определён");
        break;
    case MUSICAL_TYPE_OUT_OF_BOUNDS:
        puts("Ошибка тип музыкального спектакля по данному номеру не "
             "определён");
        break;
    case BAD_AGE_RESTRICTION:
        puts("Ошибка такое возрастное ограничение не поддерживается");
        break;
    case BAD_PLAY_LENGTH:
        puts("Ошибка длина спектакля вне пределов");
        break;
    default:
        puts("Неизвестная ошибка в считанной записи");
        break;
    }
}

int validate_table(record_table_t *table, char *filename)
{
    bool bad_indexes[table->el_count];
    int rc = 0;
    size_t bad_count = 0;

    for (size_t i = 0; i < table->el_count; ++i)
    {
        rc = validate_record(table->dataptr + i);
        if (rc)
        {
            bad_indexes[i] = true;
            printf("Запись с номером %zu: ", i + 1);
            verbose_record_error(rc);
            ++bad_count;
        }
        else
            bad_indexes[i] = false;
    }

    if (bad_count)
    {
        printf("Удалить записи с ошибками? [N/y]\n>");
        char tmp = getchar();
        clear_stdin();

        if (tmp != 'y' && tmp != 'Y')
            return EXIT_SUCCESS;

        for (size_t i = table->el_count; i > 0; --i)
            if (bad_indexes[i - 1])
            {
                rec_table_del(table, i - 1);
                --table->el_count;
            }

        rc = table_fwrite(table, filename);
        if (rc)
            return rc;
    }

    return EXIT_SUCCESS;
}

int rec_table_del(record_table_t *table, size_t index)
{
    if (index <= table->el_count)
    {
        if (index != table->el_count - 1)
            memcpy(&table->dataptr[index], &table->dataptr[index + 1],
            sizeof(record_t) * (table->el_count - index - 1));
        --table->el_count;
    }
    return EXIT_SUCCESS;
}

int table_fwrite(record_table_t *table, char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
        return BAD_FILENAME;

    fseek(f, 0, SEEK_SET);
    fwrite(table->dataptr, sizeof(record_t), table->el_count, f);

    fclose(f);

    return EXIT_SUCCESS;
}

bool check_key(int32_t key)
{
    return (key > 0 && key < KEY_N);
}

int swap(void *e1, void *e2, size_t size)
{
    char buf[size];

    memcpy(buf, e1, size);
    memcpy(e1, e2, size);
    memcpy(e2, buf, size);

    return EXIT_SUCCESS;
}

void mysort(
void *base, size_t num, size_t size, int (*compare)(const void *, const void *))
{
    void *buf = calloc(1, size);

    if (buf == NULL)
        return;

    char *cbase = (char *)base;

    for (size_t i = 0; i < num; ++i)
    {
        memcpy(buf, base, size);
        size_t max_offset = 0;

        for (char *pcur = cbase; (pcur - cbase) / size < num - i; pcur += size)
            if (compare(pcur, buf) > 0)
            {
                max_offset = (pcur - cbase);
                memcpy(buf, pcur, size);
            }

        swap(cbase + size * (num - i - 1), cbase + max_offset, size);
    }

    free(buf);
}

int sort_table(record_table_t *table, int32_t key)
{
    if (!check_key(key))
        return EXIT_FAILURE;

    if (key == T_NAME)
        mysort(
        table->dataptr, table->el_count, sizeof(record_t), table_thname_cmp);
    if (key == MIN_PRICE)
        mysort(
        table->dataptr, table->el_count, sizeof(record_t), table_minprice_cmp);

    return EXIT_SUCCESS;
}

int form_key_table(record_table_t *src, key_t **key_table, int32_t key)
{
    if (!check_key(key))
        return EXIT_FAILURE;

    *key_table = calloc(sizeof(key_t), src->el_count);
    if (!key_table)
        return NOT_ENOUGH_MEMORY;

    if (key == T_NAME)
        for (size_t i = 0; i < src->el_count; ++i)
        {
            (*key_table)[i].main_index = i;
            (*key_table)[i].value.text = &src->dataptr[i].theatre_name[0];
        }
    else if (key == MIN_PRICE)
        for (size_t i = 0; i < src->el_count; ++i)
        {
            (*key_table)[i].main_index = i;
            (*key_table)[i].value.num = src->dataptr[i].ticket_prices.min_price;
        }

    return EXIT_SUCCESS;
}

int sort_key_table(record_table_t *table, key_t *key_table, int32_t key)
{
    if (!check_key(key))
        return EXIT_FAILURE;

    if (key == T_NAME)
        mysort(key_table, table->el_count, sizeof(key_t), key_thname_cmp);
    else if (key == MIN_PRICE)
        mysort(key_table, table->el_count, sizeof(key_t), key_minprice_cmp);

    return EXIT_SUCCESS;
}

int table_thname_cmp(const void *l, const void *r)
{
    char *pl = ((record_t *)l)->theatre_name,
         *pr = ((record_t *)r)->theatre_name;

    return strcmp(pl, pr);
}

int key_thname_cmp(const void *l, const void *r)
{
    char *pl = ((key_t *)l)->value.text, *pr = ((key_t *)r)->value.text;

    return strcmp(pl, pr);
}

int table_minprice_cmp(const void *l, const void *r)
{
    int32_t pl = ((record_t *)l)->ticket_prices.min_price;
    int32_t pr = ((record_t *)r)->ticket_prices.min_price;

    return pl - pr;
}

int key_minprice_cmp(const void *l, const void *r)
{
    int32_t pl = ((key_t *)l)->value.num;
    int32_t pr = ((key_t *)r)->value.num;

    return pl - pr;
}

int copy_table(record_table_t *src, record_table_t *dst)
{
    int rc;
    rc = rec_table_init(dst, src->el_count);
    if (rc)
        return rc;

    memcpy(dst->dataptr, src->dataptr, sizeof(record_t) * src->el_count);
    dst->el_count = src->el_count;
    return EXIT_SUCCESS;
}

int filter_table(
record_table_t *src, bool *dst, int32_t age, int32_t max_length)
{
    size_t count = 0;
    for (size_t i = 0; i < src->el_count; ++i)
    {
        if (src->dataptr[i].play_type == MUSIC &&
            src->dataptr[i].play_attrs.musical_attrs.type == BALLET)
            if (src->dataptr[i].play_attrs.musical_attrs.age_restriction <= age)
                if (src->dataptr[i].play_attrs.musical_attrs.length <
                    max_length)
                {
                    ++count;
                    dst[i] = true;
                    continue;
                }
        dst[i] = false;
    }
    return count;
}
