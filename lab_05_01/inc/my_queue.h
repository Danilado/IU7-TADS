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

static int QUEUE_VERBOSE_PUSH;
static int QUEUE_VERBOSE_POP;
typedef struct queue *queue_t;

void set_verbose_pop(enum q_verbose_state state);
void set_verbose_push(enum q_verbose_state state);

queue_t queue_create(void);
void queue_destroy(queue_t *q);
void queue_clear(queue_t q);

int queue_push(queue_t q, void *value);
int queue_priority_push(queue_t q, void *value, size_t max_pos);
void *queue_pop(queue_t q);

size_t queue_get_length(const queue_t q);

void queue_apply_darg(queue_t q,
void (*func)(node_t *el, void *arg1, void *arg2), void *arg1, void *arg2);

#endif
