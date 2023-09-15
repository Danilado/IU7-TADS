#include "bignum.h"

int exp_check(int32_t value)
{
    if (value > EXP_ABS_LIMIT)
        return EXPONENT_TOO_BIG;

    if (value < -EXP_ABS_LIMIT)
        return EXPONENT_TOO_SMALL;

    return EXIT_SUCCESS;
}

// НЕ МЕНЯЕТ ЭКСПОНЕНТУ!
// НЕ ОКРУГЛЯЕТ НИЧЕГО!
// РИСК ПОТЕРИ ДАННЫХ!
void shift_mantissa(bignum_t *num, size_t amount)
{
    if (amount == 0)
        return;

    for (size_t i = MANTISSA_LIMIT - 1; i >= amount; --i)
        num->mantissa[i] = num->mantissa[i - amount];

    for (size_t i = 0; i < amount; ++i)
        num->mantissa[i] = 0;
}

void shift_mantissa_to_end(bignum_t *num, size_t mantissa_len)
{
    shift_mantissa(num, MANTISSA_LIMIT - mantissa_len);
}

void verbose_error(int rc)
{
    switch (rc)
    {
    case MISSING_SIGN:
        puts("Ввод знака обязателен");
        break;
    case MANTISSA_TOO_LONG:
        puts("Длина мантиссы превысила предел");
        break;
    case UNSUPPORTED_CHARACTER:
        puts("Встретился непонятный символ");
        break;
    case EXPONENT_TOO_BIG:
        puts("Достигнута машинная бесконечность");
        break;
    case EXPONENT_TOO_SMALL:
        puts("Достигнут машинный 0");
        break;
    case EXPONENT_TOO_BIG + 10:
        puts("Длина экспоненты превысила предел");
        break;
    case EXPONENT_TOO_SMALL + 10:
        puts("Длина экспоненты превысила предел");
        break;
    default:
        puts("Ошибка ввода без описания, интересно как это вообще");
        break;
    }
}

// Вот тут начинается считывание

int get_sign(const char *cptr, bool *dst)
{
    // clang-format off
    switch (*cptr)
    {
        case '+':
            *dst = false;
            break;

        case '-':
            *dst = true;
            break;

        default:
            return MISSING_SIGN;
    }
    // clang-format on
    return EXIT_SUCCESS;
}

int read_integer_part(
const char **pcur, bignum_t *num, size_t max_mantissa, size_t *cur_mantissa_len)
{
    do
    {
        ++*pcur;

        // сработает только если первый символ в
        // строке после знака - точка
        if (**pcur == '.' || **pcur == '\0' ||
            strspn(*pcur, EXP_CHARACTERS) == 1)
            break;

        // сработает только если первый символ в
        // строке после знака не цифра (и не точка)
        if (!isdigit(**pcur))
            return UNSUPPORTED_CHARACTER;

        if (**pcur == '0' && *cur_mantissa_len == 0)
            continue;

        if (*cur_mantissa_len > max_mantissa)
            return MANTISSA_TOO_LONG;

        num->mantissa[*cur_mantissa_len] = **pcur - '0';
        ++*cur_mantissa_len;
    } while (isdigit(**pcur));

    return EXIT_SUCCESS;
}

int read_float_part(const char **pcur, bignum_t *num, size_t max_mantissa,
size_t *cur_mantissa_len, size_t *zero_count)
{
    int32_t trailing_zero_counter = 0;

    do
    {
        ++*pcur;

        if (strspn(*pcur, EXP_CHARACTERS) || **pcur == '\0')
            break;

        // сработает только если сразу
        // после точки не пойми что
        if (!isdigit(**pcur))
            return UNSUPPORTED_CHARACTER;

        if (**pcur == '0')
            ++trailing_zero_counter;
        else
        {
            if (*zero_count == 0)
                *zero_count = trailing_zero_counter;
            trailing_zero_counter = 0;
        }

        if (*cur_mantissa_len > max_mantissa)
            return MANTISSA_TOO_LONG;

        num->mantissa[*cur_mantissa_len] = **pcur - '0';
        ++*cur_mantissa_len;
    } while (isdigit(**pcur));

    *cur_mantissa_len -= trailing_zero_counter;

    return EXIT_SUCCESS;
}

int bignum_sscan(const char *src, bignum_t *num, size_t max_mantissa)
{
    const char *pcur = src;
    int rc = 0;

    rc = get_sign(pcur, &num->is_negative);
    if (rc)
        return rc;

    size_t cur_mantissa_len = 0;
    int32_t exp_part = 0;

    rc = read_integer_part(&pcur, num, max_mantissa, &cur_mantissa_len);
    if (rc)
        return rc;

    // сохраняем положительную часть экспоненты
    // по идее когда мантисса будет в дробной части
    // и мы всё вычтем, будет всё как раз так, как надо,
    // если вычесть то, что мы сохранили сейчас из полной
    // длины и домножить там на -1
    exp_part += cur_mantissa_len;

    if (*pcur == '\0')
    {
        shift_mantissa_to_end(num, cur_mantissa_len);
        num->exponent = exp_part;
        return EXIT_SUCCESS;
    }

    if (*pcur == '.')
    {
        size_t zero_count = 0;
        rc = read_float_part(
        &pcur, num, max_mantissa, &cur_mantissa_len, &zero_count);
        if (rc)
            return rc;
        if (exp_part == 0)
            exp_part -= zero_count;
    }
    else if (strspn(pcur, EXP_CHARACTERS) != 1)
        return UNSUPPORTED_CHARACTER;

    if (*pcur == '\0')
    {
        shift_mantissa_to_end(num, cur_mantissa_len);
        num->exponent = exp_part;
        return EXIT_SUCCESS;
    }

    else if (strspn(pcur, EXP_CHARACTERS) != 1)
        return UNSUPPORTED_CHARACTER;

    // уходим с ешки
    ++pcur;

    int32_t exp_in_str;
    double tmp;

    sscanf(pcur, "%lf", &tmp);
    if (tmp != (double)(int)(tmp))
        return UNSUPPORTED_CHARACTER;

    if (sscanf(pcur, "%" SCNd32 "\n", &exp_in_str) != 1)
        return UNSUPPORTED_CHARACTER;

    rc = exp_check(exp_in_str);
    if (rc)
        return rc;

    exp_part += exp_in_str;
    shift_mantissa_to_end(num, cur_mantissa_len);
    num->exponent = exp_part;

    return EXIT_SUCCESS;
}

// Вот тут заканчивается считывание

// много минусов, нет проверок на переполнение
int bignum_to_string(char *dst, const bignum_t *num)
{
    if (num->is_negative)
        sprintf(dst, "-");
    else
        sprintf(dst, "+");

    sprintf(dst, "0.");
    bool zero_passed_flag = false;

    for (size_t i = 0; i < MANTISSA_LIMIT; ++i)
    {
        if (num->mantissa[i] == 0 && !zero_passed_flag)
            continue;

        zero_passed_flag = true;
        sprintf(dst, "%d", (int)num->mantissa[i]);
    }

    sprintf(dst, "E%0+6d", num->exponent);

    return EXIT_SUCCESS;
}

// не делает \n
void bignum_print(const bignum_t *num)
{
    if (num->is_negative)
        printf("-");
    else
        printf("+");

    printf("0.");
    bool zero_passed_flag = false;

    for (size_t i = 0; i < MANTISSA_LIMIT; ++i)
    {
        if (num->mantissa[i] == 0 && !zero_passed_flag)
            continue;

        zero_passed_flag = true;
        printf("%d", (int)num->mantissa[i]);
    }

    if (!zero_passed_flag)
        printf("0");

    printf("E%0+6d", num->exponent);
}

// на самом деле считывают в строку и делает sscan
int bignum_scan(bignum_t *dst, size_t max_mantissa, bool silent)
{
    int rc;
    for (size_t i = 0; i < 3; ++i)
    {
        if (!silent)
        {
            printf(PROMPT, max_mantissa);
            printf(RULER);
        }

        char tmp_string[MANTISSA_LIMIT + 10] = "";

        if (fgets(tmp_string, MANTISSA_LIMIT + 10, stdin) == NULL)
            return SCAN_ERROR;

        tmp_string[strcspn(tmp_string, "\n")] = '\0';

        rc = bignum_sscan(tmp_string, dst, max_mantissa);
        if (!rc)
            break;

        // чистим ввод, если юзер ввёл какое-то
        // невероятное количество символов
        if (strlen(tmp_string) == MANTISSA_LIMIT + 10)
        {
            char c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
        if (!silent)
        {
            if (rc == EXPONENT_TOO_BIG || rc == EXPONENT_TOO_SMALL)
                rc += 10;
            verbose_error(rc);
        }
    }

    if (rc)
        return rc;

    return EXIT_SUCCESS;
}

bignum_t double_to_bignum(double num)
{
    char stringified_double[MANTISSA_LIMIT + 1] = "";
    snprintf(stringified_double, MANTISSA_LIMIT, "%+.15E", num);

    bignum_t result;
    bignum_sscan(stringified_double, &result, MANTISSA_LIMIT);

    return result;
}

// Умножение

int get_dig_amount(bignum_t *num)
{
    int count = 0;
    bool non_zero_found = false;

    for (int i = 0; i < MANTISSA_LIMIT; ++i)
        if (num->mantissa[i] == 0 && !non_zero_found)
            continue;
        else
        {
            non_zero_found = true;
            ++count;
        }

    return count;
}

void shift_overflow(unsigned char *s, unsigned char *f)
{
    for (unsigned char *pcur = f; pcur > s; --pcur)
    {
        unsigned char tmp = *pcur / 10;
        *pcur %= 10;
        *(pcur - 1) += tmp;
    }
}

// возвращает начало и конец мантиссы для нашего типа в большом массиве
// и округляет там, где надо
void form_mantissa_in_tmp_type(unsigned char *arr, size_t *s, size_t *f)
{
    while (!arr[*s])
        ++*s;

    *f = *s + MANTISSA_LIMIT;
    if (*f >= 2 * MANTISSA_LIMIT)
        *f = 2 * MANTISSA_LIMIT - 1;
    else
    {
        arr[*f] += arr[*f + 1] / 5;
        shift_overflow(arr, arr + MANTISSA_LIMIT * 2 - 1);
    }
}

int bignum_mul(bignum_t *num1, bignum_t *num2, bignum_t *dst)
{
    // если что-то нолик
    if (!(get_dig_amount(num1) && get_dig_amount(num2)))
    {
        dst->is_negative = false;
        dst->exponent = 0;
        shift_mantissa(dst, MANTISSA_LIMIT);
        return EXIT_SUCCESS;
    }

    // при умножении двух целых чисел, значащих разрядов в результате будет
    // не больше, чем сумма значащих разрядов исходных чисел
    unsigned char tmp_arr[MANTISSA_LIMIT * 2] = {0};

    // идём с конца => надо иметь в сумме длину доп массива
    // 40+40 = 80   -1 = 79
    // переполнения в j+i-1 не будет, потому что при умножении
    // двух первых элементов они запишутся в элемент с индексом 1,
    // как и положено при умножении в столбик, а вот переполнение
    // самой ячейки уже пойдёт в нулевую ячейку, где ей и место.
    for (int i = MANTISSA_LIMIT; i > 0; --i)
    {
        for (int j = MANTISSA_LIMIT; j > 0; --j)
            tmp_arr[j + i - 1] += num1->mantissa[i - 1] * num2->mantissa[j - 1];

        shift_overflow(tmp_arr, tmp_arr + MANTISSA_LIMIT * 2 - 1);
    }

    size_t non_zero_index = 0, last_index;

    form_mantissa_in_tmp_type(tmp_arr, &non_zero_index, &last_index);

    int32_t new_exp = num1->exponent + num2->exponent;
    if ((size_t)(get_dig_amount(num1) + get_dig_amount(num2)) ==
        (MANTISSA_LIMIT * 2 - non_zero_index + 1))
        --new_exp;

    int rc = exp_check(new_exp);
    if (rc)
        return rc;

    if (tmp_arr[non_zero_index - 1] != 0)
    {
        --non_zero_index;
        --last_index;
    }

    for (size_t i = non_zero_index; i <= last_index; ++i)
        dst->mantissa[i - non_zero_index] = tmp_arr[i];

    dst->exponent = new_exp;
    dst->is_negative = num1->is_negative ^ num2->is_negative;

    return EXIT_SUCCESS;
}
