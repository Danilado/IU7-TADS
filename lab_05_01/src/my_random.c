#include "my_random.h"

double my_rand(void)
{
    return ((double)random()) / ((double)LONG_MAX);
}
