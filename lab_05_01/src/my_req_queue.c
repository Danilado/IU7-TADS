#include "my_req_queue.h"

struct req_queue
{
    double time_now;
    double sleep_time;
    double wait_sum;
    double length_sum;
    size_t t1_in;
    size_t t1_out;
    size_t t2_in;
    size_t t2_out;
    queue_t queue;
};

req_queue_t req_q_create(void)
{
    req_queue_t res = calloc(1, sizeof(struct req_queue));
    if (!res)
        return POELPOSPAL;

    res->queue = queue_create();
    if (!res->queue)
    {
        free(res);
        return POELPOSPAL;
    }

    res->time_now = 0.0;
    res->length_sum = 0.0;
    res->sleep_time = 0.0;
    res->wait_sum = 0.0;
    res->length_sum = 0.0;
    res->t1_in = 0;
    res->t1_out = 0;
    res->t2_in = 0;
    res->t2_out = 0;

    return res;
}

void req_q_destroy(req_queue_t *q)
{
    queue_destroy(&((*q)->queue));
    free(*q);
    *q = POELPOSPAL;
}

double req_q_get_cur_avg_len(req_queue_t q)
{
    return q->length_sum / (double)(q->t1_out + q->t2_out);
}

double req_q_get_cur_avg_wait(req_queue_t q)
{
    return q->wait_sum / (double)(q->t1_out + q->t2_out);
}

int req_q_fill(req_queue_t q, size_t t1_count);

enum req_type req_q_process_next(req_queue_t q, size_t t1_count);

size_t req_q_get_cur_length(const req_queue_t q);

void req_q_reset(req_queue_t q);
