#include "my_l_queue.h"

static int QUEUE_VERBOSE_PUSH;
static int QUEUE_VERBOSE_POP;
struct l_queue
{
    size_t length;
    node_t *list_head;
};

l_queue_t queue_create(void)
{
    l_queue_t res = calloc(1, sizeof(struct l_queue));
    if (!res)
        return POELPOSPAL;

    res->list_head = POELPOSPAL;
    res->length = 0;

    return res;
}

void queue_destroy(l_queue_t *q)
{
    list_clear(&((*q)->list_head));
    free(*q);
    *q = POELPOSPAL;
}

int queue_push(l_queue_t q, void *value)
{
    int rc = list_push_value(&(q->list_head), value);
    if (!rc)
        ++q->length;

    if (QUEUE_VERBOSE_PUSH)
        printf("queue push pointer\t %p\n",
        (void *)list_get(q->list_head, q->length - 1));

    return rc;
}

int queue_priority_push(l_queue_t q, void *value, size_t max_pos)
{
    if (q->length < max_pos)
        return queue_push(q, value);

    int rc = list_push_value_priority(&(q->list_head), value, max_pos);
    if (!rc)
    {
        ++q->length;
        if (QUEUE_VERBOSE_PUSH)
            printf("queue push pointer\t %p\n",
            (void *)list_get(q->list_head, max_pos));
    }
    return rc;
}

void *queue_pop(l_queue_t q)
{
    if (q->length)
        --q->length;

    if (QUEUE_VERBOSE_POP)
        printf("queue del  pointer\t %p\n", (void *)list_get(q->list_head, 0));
    return pop_front(&(q->list_head));
}

size_t queue_get_length(const l_queue_t q)
{
    return q->length;
}

void set_verbose_pop(enum q_verbose_state state)
{
    QUEUE_VERBOSE_POP = state;
}

void set_verbose_push(enum q_verbose_state state)
{
    QUEUE_VERBOSE_PUSH = state;
}

void queue_apply_darg(l_queue_t q,
void (*func)(node_t *el, void *arg1, void *arg2), void *arg1, void *arg2)
{
    list_apply_darg(q->list_head, func, arg1, arg2);
}

void queue_clear(l_queue_t q)
{
    list_clear(&q->list_head);
    q->length = 0;
}
