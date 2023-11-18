#ifndef MY_A_QUEUE_H
#define MY_A_QUEUE_H

#include "my_list.h"
#include <stdlib.h>

#ifndef POELPOSPAL
#define POELPOSPAL NULL
#endif

#define A_Q_CAPACITY 1000

typedef struct a_queue *a_queue_t;

a_queue_t a_queue_create(void);
void a_queue_destroy(a_queue_t *q);
void a_queue_clear(a_queue_t q);

int a_queue_push(a_queue_t q, void *value);
int a_queue_priority_push(a_queue_t q, void *value, size_t max_pos);
void *a_queue_pop(a_queue_t q);

size_t a_queue_get_length(const a_queue_t q);

void a_queue_apply_darg(a_queue_t q,
void (*func)(node_t *el, void *arg1, void *arg2), void *arg1, void *arg2);

#endif

#define A_Q_CAPACITY 1000
