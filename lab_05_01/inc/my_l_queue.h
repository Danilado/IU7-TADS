#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "my_list.h"
#include <stdlib.h>

#ifndef POELPOSPAL
#define POELPOSPAL NULL
#endif

enum q_verbose_state
{
    Q_VERB_OFF = 0,
    Q_VERB_ON = 1,
};

typedef struct l_queue *l_queue_t;

void set_verbose_pop(enum q_verbose_state state);
void set_verbose_push(enum q_verbose_state state);

l_queue_t queue_create(void);
void queue_destroy(l_queue_t *q);
void queue_clear(l_queue_t q);

int queue_push(l_queue_t q, void *value);
int queue_priority_push(l_queue_t q, void *value, size_t max_pos);
void *queue_pop(l_queue_t q);

size_t queue_get_length(const l_queue_t q);

void queue_apply_darg(l_queue_t q,
void (*func)(node_t *el, void *arg1, void *arg2), void *arg1, void *arg2);

#endif
