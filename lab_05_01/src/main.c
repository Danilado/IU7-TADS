#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef POSIX_C_SOURCE
#define POSIX_C_SOURCE 200809L
#endif

#include "my_queue.h"
#include "my_random.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL) + 42);
    return EXIT_SUCCESS;
}
