#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <sys/time.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)
#define time_now(x) clock_gettime(CLOCK_REALTIME, &x);

long delta_time(struct timespec mt1, struct timespec mt2);
void clear_stdin(void);

#endif
