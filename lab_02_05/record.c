#include "record.h"

char *musical_type_str[4] = {
"---",
"BALLET",
"OPERA",
"MUSICAL",
};

char *play_type_str[6] = {
"---", "PIECE", "DRAMA", "COMEDY", "FAIRYTALE", "MUSICAL"};

int check_age(int32_t age)
{
    if (age != 3 && age != 10 && age != 16)
        return BAD_AGE_RESTRICTION;
    return EXIT_SUCCESS;
}

int check_length(int32_t length)
{
    if (length < 0 || length > 1440)
        return BAD_PLAY_LENGTH;
    return EXIT_SUCCESS;
}

int check_play_type(int32_t input)
{
    if (input < 1 || input >= PLAY_TYPE_N)
        return PLAY_TYPE_OUT_OF_BOUNDS;
    return EXIT_SUCCESS;
}

int check_musical_type(int32_t input)
{
    if (input < 1 || input >= MUSICAL_TYPE_N)
        return MUSICAL_TYPE_OUT_OF_BOUNDS;
    return EXIT_SUCCESS;
}

int get_name_t_from_buf(name_t *dst, char *buf)
{
    if (strlen(buf) > MAX_NAME_LEN)
        return STRING_TOO_LONG;

    if (strlen(buf) == 0)
        return NO_STRING;

    strncpy(*dst, buf, MAX_NAME_LEN);
    return EXIT_SUCCESS;
}

int check_prices(int32_t a, int32_t b)
{
    if (a < 0 || b < 0)
        return PRICE_LOWER_THAN_ZERO;

    if (b < a)
        return PRICE_MINMAX_MISMATCH;

    return 0;
}

void verbose_scan_error(int rc)
{
    switch (rc)
    {
    case STRING_TOO_LONG:
        puts("Введённая строка слишком длинная. Предел - 20 символов.");
        break;
    case NO_STRING:
        puts("Введите что-нибудь.");
        break;
    case BAD_NUMBER:
        puts("Ошибка ввода числа");
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
    case PRICE_LOWER_THAN_ZERO:
        puts("Цена не может быть ниже нуля");
        break;
    case PRICE_MINMAX_MISMATCH:
        puts("Диапазон цен должен быть неубывающим");
        break;
    default:
        puts("Неизвестная ошибка со вводом строки");
        break;
    }
}

int record_scan(record_t *record)
{
    int rc = 0;
    char input_buf[30];

    record_init(record);
    puts("Считывание новой записи");

    do
    {
        if (rc)
            verbose_scan_error(rc);
        printf("Введите название театра\n> ");
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strcspn(input_buf, "\n")] = '\0';

        rc = get_name_t_from_buf(&record->theatre_name, input_buf);

        if (rc == STRING_TOO_LONG)
            clear_stdin();
    } while (rc);

    do
    {
        if (rc)
            verbose_scan_error(rc);
        printf("Введите название спектакля\n> ");
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strcspn(input_buf, "\n")] = '\0';

        rc = get_name_t_from_buf(&record->play_name, input_buf);

        if (rc == STRING_TOO_LONG)
            clear_stdin();
    } while (rc);

    do
    {
        if (rc)
            verbose_scan_error(rc);
        printf("Введите диапазон цен билетов\n> ");

        int32_t s, e;
        if (scanf("%" SCNd32, &s) != 1)
        {
            rc = BAD_NUMBER;
            clear_stdin();
            continue;
        }

        if (scanf("%" SCNd32, &e) != 1)
        {
            rc = BAD_NUMBER;
            clear_stdin();
            continue;
        }

        clear_stdin();

        rc = check_prices(s, e);

        if (!rc)
        {
            record->ticket_prices.min_price = s;
            record->ticket_prices.max_price = e;
        }

    } while (rc);

    do
    {
        if (rc)
            verbose_scan_error(rc);

        printf("Введите тип спектакля\n");
        printf("\t1: Пьеса\n");
        printf("\t2: Драма\n");
        printf("\t3: Комедия\n");
        printf("\t4: Сказка\n");
        printf("\t5: Музыкальный\n");
        printf("> ");

        int32_t tmp;
        if (scanf("%" SCNd32, &tmp) != 1)
        {
            rc = BAD_NUMBER;
            clear_stdin();
            continue;
        }
        clear_stdin();

        rc = check_play_type(tmp);

        if (!rc)
            record->play_type = (unsigned char)tmp;

    } while (rc);

    if (record->play_type == FAIRYTALE)
    {
        do
        {
            if (rc)
                verbose_scan_error(rc);

            printf("Введите возрастное ограничение сказки\n");
            printf("(только число), поддерживаются 3, 10 и 16\n");
            printf("> ");

            int32_t tmp;
            if (scanf("%" SCNd32, &tmp) != 1)
            {
                rc = BAD_NUMBER;
                clear_stdin();
                continue;
            }
            clear_stdin();

            rc = check_age(tmp);

            if (!rc)
                record->play_attrs.age_restriction = (unsigned char)tmp;
        } while (rc);
    }
    else if (record->play_type == MUSIC)
    {
        do
        {
            if (rc)
                verbose_scan_error(rc);

            printf("Введите тип музыкального спектакля\n");
            printf("\t1: Балет\n");
            printf("\t2: Опера\n");
            printf("\t3: Мюзикл\n");
            printf("> ");

            int32_t tmp;
            if (scanf("%" SCNd32, &tmp) != 1)
            {
                rc = BAD_NUMBER;
                clear_stdin();
                continue;
            }
            clear_stdin();

            rc = check_musical_type(tmp);

            if (!rc)
                record->play_attrs.musical_attrs.type = (unsigned char)tmp;

        } while (rc);

        do
        {
            if (rc)
                verbose_scan_error(rc);
            printf("Введите композитора\n> ");
            fgets(input_buf, sizeof(input_buf), stdin);
            input_buf[strcspn(input_buf, "\n")] = '\0';

            rc = get_name_t_from_buf(
            &record->play_attrs.musical_attrs.composer, input_buf);

            if (rc == STRING_TOO_LONG)
                clear_stdin();

        } while (rc);

        do
        {
            if (rc)
                verbose_scan_error(rc);
            printf("Введите страну\n> ");
            fgets(input_buf, sizeof(input_buf), stdin);
            input_buf[strcspn(input_buf, "\n")] = '\0';

            rc = get_name_t_from_buf(
            &record->play_attrs.musical_attrs.country, input_buf);

            if (rc == STRING_TOO_LONG)
                clear_stdin();

        } while (rc);

        rc = 0;

        do
        {
            if (rc)
                verbose_scan_error(rc);

            printf("Введите возрастное ограничение выступления\n");
            printf("(только число), поддерживаются 3, 10 и 16\n");
            printf("> ");

            int32_t tmp;
            if (scanf("%" SCNd32, &tmp) != 1)
            {
                rc = BAD_NUMBER;
                clear_stdin();
                continue;
            }
            clear_stdin();

            rc = check_age(tmp);

            if (!rc)
                record->play_attrs.musical_attrs.age_restriction =
                (unsigned char)tmp;
        } while (rc);

        do
        {
            if (rc)
                verbose_scan_error(rc);

            printf("Введите длину выступления в минутах\n");
            printf("> ");

            int32_t tmp;
            if (scanf("%" SCNd32, &tmp) != 1)
            {
                rc = BAD_NUMBER;
                clear_stdin();
                continue;
            }
            clear_stdin();

            rc = check_length(tmp);

            if (!rc)
                record->play_attrs.musical_attrs.length = (short)tmp;
        } while (rc);
    }

    return EXIT_SUCCESS;
}

void record_init(record_t *record)
{
    char *record_ptr = (char *)record;

    for (size_t i = 0; i < sizeof(record_t); ++i)
        record_ptr[i] = 0;
}

void record_print(record_t *record, size_t index)
{
    // Глобально для всех спектаклей
    printf("│ %6zu │ %20s │ %20s │ %8" PRId32 "-%-8" PRId32 " │ %13s │ ", index,
    record->theatre_name, record->play_name, record->ticket_prices.min_price,
    record->ticket_prices.max_price, play_type_str[record->play_type]);

    // остальное
    printf("%10s │ ", musical_type_str[record->play_attrs.musical_attrs.type]);

    char *tmp;

    if (strlen(record->play_attrs.musical_attrs.composer))
        tmp = record->play_attrs.musical_attrs.composer;
    else
        tmp = "---";
    printf("%20s │ ", tmp);

    if (strlen(record->play_attrs.musical_attrs.country))
        tmp = record->play_attrs.musical_attrs.country;
    else
        tmp = "---";
    printf("%20s │ ", tmp);

    printf("%6d+ │ ", record->play_attrs.age_restriction);

    if (record->play_attrs.musical_attrs.length)
        printf("%16d │", record->play_attrs.musical_attrs.length);
    else
        printf("%16s │", "---");

    printf("\n");
}

int validate_record(record_t *record)
{
    int rc;

    if (strlen(record->theatre_name) == 0)
        return NO_STRING;
    if (strlen(record->theatre_name) > MAX_NAME_LEN)
        return STRING_FIELDS_OVERLAP;

    if (strlen(record->play_name) == 0)
        return NO_STRING;
    if (strlen(record->play_name) > MAX_NAME_LEN)
        return STRING_FIELDS_OVERLAP;

    if (check_play_type(record->play_type))
        return PLAY_TYPE_OUT_OF_BOUNDS;

    if (record->play_type == MUSIC)
    {
        if (check_musical_type(record->play_attrs.musical_attrs.type))
            return MUSICAL_TYPE_OUT_OF_BOUNDS;

        if (strlen(record->play_attrs.musical_attrs.composer) == 0)
            return NO_STRING;
        if (strlen(record->play_attrs.musical_attrs.composer) > MAX_NAME_LEN)
            return STRING_FIELDS_OVERLAP;

        if (strlen(record->play_attrs.musical_attrs.country) == 0)
            return NO_STRING;
        if (strlen(record->play_attrs.musical_attrs.country) > MAX_NAME_LEN)
            return STRING_FIELDS_OVERLAP;

        if (check_age(record->play_attrs.age_restriction))
            return BAD_AGE_RESTRICTION;
    }
    if (record->play_type == FAIRYTALE)
    {
        if (check_age(record->play_attrs.age_restriction))
            return BAD_AGE_RESTRICTION;
    }

    rc = check_prices(
    record->ticket_prices.min_price, record->ticket_prices.max_price);
    if (rc)
        return rc;

    return EXIT_SUCCESS;
}
