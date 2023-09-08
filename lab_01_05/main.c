#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>

#define FIRST_ELEMENT_MANTISSA_LEN 35
#define SECOND_ELEMENT_MANTISSA_LEN 40

int main(void)
{
    bignum_t num1, num2, result;
    int rc;

    rc = bignum_scan(&num1, FIRST_ELEMENT_MANTISSA_LEN, false);
    if (rc)
    {
        puts("Слишком много неудачных попыток");
        return rc;
    }

    rc = bignum_scan(&num2, SECOND_ELEMENT_MANTISSA_LEN, false);
    if (rc)
    {
        puts("Слишком много неудачных попыток");
        return rc;
    }

    printf("Вы ввели:\nмножитель 1: ");
    bignum_print(&num1);
    printf("\nмножитель 2: ");
    bignum_print(&num2);
    printf("\n");

    rc = bignum_mul(&num1, &num2, &result);
    if (rc)
    {
        verbose_error(rc);
        return rc;
    }

    printf("результат:   ");
    bignum_print(&result);
    printf("\n");

    return EXIT_SUCCESS;
}
