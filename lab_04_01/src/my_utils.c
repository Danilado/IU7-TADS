#include "my_utils.h"

long delta_time(struct timespec mt1, struct timespec mt2)
{
    return 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
}

void clear_stdin(void)
{
    char tmp;
    while ((tmp = getchar()) != '\n' && tmp != EOF)
        ;
}
