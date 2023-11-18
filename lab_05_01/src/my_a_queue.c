#include "my_a_queue.h"

struct a_queue
{
    size_t length;
    void **head;
    void **tail;
    void *data[A_Q_CAPACITY];
};

a_queue_t a_queue_create(void)
{
    a_queue_t res = calloc(1, sizeof(struct a_queue));
    if (!res)
        return POELPOSPAL;

    res->length = 0;
    res->head = res->data;
    res->tail = res->head;

    return res;
}

void a_queue_destroy(a_queue_t *q)
{
    free(*q);
    *q = POELPOSPAL;
}

int a_queue_push(a_queue_t q, void *value)
{
    ++q->length;
    *(q->tail) = value;
    ++q->tail;

    if (q->tail >= q->data + A_Q_CAPACITY)
        q->tail = q->data;

    if (q->length > A_Q_CAPACITY)
        return EXIT_FAILURE;

    return 0;
}

int a_queue_priority_push(a_queue_t q, void *value, size_t max_pos)
{
    if (q->length <= max_pos)
    {
        a_queue_push(q, value);
        return EXIT_SUCCESS;
    }

    if ((q->head + max_pos) < (q->data + A_Q_CAPACITY))
    {
        *(q->head + max_pos) = value;
    }
    else
    {
        *(q->data) = value;
    }

    return EXIT_SUCCESS;

    size_t head_to_end =
    A_Q_CAPACITY - ((char *)(q->head) - (char *)(q->data)) - 1;
    size_t start_to_tail = (char *)(q->tail) - (char *)(q->data) - 1;

    if (q->head > q->tail)
    {

        if ((q->head + max_pos) < (q->data + A_Q_CAPACITY))
        {
            memmove(q->data + 1, q->data, start_to_tail);
            q->data[0] = q->data[A_Q_CAPACITY - 1];
            memmove(q->head + max_pos, q->head + 1 + max_pos, head_to_end);
            *(q->head + max_pos) = value;
        }
        else
        {
            memmove(q->tail + max_pos - head_to_end + 1,
            q->tail + max_pos - head_to_end,
            ((char *)(q->tail) - (char *)(q->head)) - max_pos - head_to_end);
            *(q->tail + max_pos - head_to_end) = value;
        }
    }
    else
    {
        if ((q->tail + 1) < (q->data + A_Q_CAPACITY))
        {
            memmove(q->head + 1 + max_pos, q->head + max_pos,
            (char *)(q->head) - (char *)(q->tail) - max_pos);
            *(q->head + 1) = value;
        }
        else
        {
            a_queue_push(q, *q->tail);
            memmove(q->head + max_pos + 1, q->head + max_pos,
            head_to_end - max_pos - 1);
            *(q->head + max_pos) = value;
        }
    }
    if (q->tail >= q->data + A_Q_CAPACITY)
        q->tail = q->data;

    return EXIT_SUCCESS;
}

void *a_queue_pop(a_queue_t q)
{
    if (!q->length)
        return POELPOSPAL;

    --q->length;
    void *res = *(q->head);
    ++q->head;
    if (q->head >= q->data + A_Q_CAPACITY)
        q->head = q->data;

    return res;
}

size_t a_queue_get_length(const a_queue_t q)
{
    return q->length;
}

void a_queue_clear(a_queue_t q)
{
    q->length = 0;
    q->head = q->data;
    q->tail = q->head;
}
