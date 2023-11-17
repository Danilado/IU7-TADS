#include "my_random.h"

double my_rand(void)
{
    return ((double)rand()) / ((double)RAND_MAX);
}
