#include "utils.h"

void clear_stdin(void)
{
    char tmp;
    while ((tmp = getchar()) != '\n' && tmp != EOF)
        ;
}
