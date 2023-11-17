#include "my_req_queue.h"

#ifndef MIN
#define MIN(x, y) ((x < y) ? x : y)
#endif
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
    req_q_reset(*q);
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

size_t req_q_fill(req_queue_t q, size_t t1_count)
{
    double timeline = 0.0;

    {
        request_t tmp = req_create(TYPE_TWO);
        if (!tmp)
            return 0;
        queue_push(q->queue, tmp);
    }

    for (size_t i = 0; i < t1_count; ++i)
    {
        request_t tmp = req_create(TYPE_ONE);
        if (!tmp)
            return i;
        double tmptime = req_get_arrival_time(tmp);
        req_time_add(tmp, timeline);
        timeline += tmptime;
        queue_push(q->queue, tmp);
    }

    return t1_count;
}

enum req_type req_q_process_next(req_queue_t q)
{
    request_t tmp = queue_pop(q->queue);
    if (!tmp)
        return NO_REQUEST;

    enum req_type tmptype = req_get_type(tmp);
    if (tmptype == TYPE_TWO)
        q->t2_in++;
    else
        q->t1_in++;

    q->length_sum += req_q_get_cur_length(q);

    if (q->time_now < req_get_arrival_time(tmp))
    {
        q->sleep_time += req_get_arrival_time(tmp) - q->time_now;
        q->time_now = req_get_arrival_time(tmp);
    }
    else if (q->time_now > req_get_arrival_time(tmp))
    {
        q->wait_sum += q->time_now - req_get_arrival_time(tmp);
    }

    double worktime = my_rand() * MAX_WORK_TIME;
    q->time_now += worktime;

    if (tmptype == TYPE_TWO)
    {
        req_time_add(tmp, worktime);
        queue_priority_push(q->queue, tmp, MIN(4, req_q_get_cur_length(q)));
        q->t2_out += 1;
    }
    else
    {
        req_destroy(&tmp);
        q->t1_out += 1;
    }

    return tmptype;
}

void count_arrived(node_t *el, void *timestamp, void *counter)
{
    request_t req = el->data;
    if (req_get_arrival_time(req) <= *(double *)timestamp)
        ++*(size_t *)counter;
}

size_t req_q_get_cur_length(const req_queue_t q)
{
    size_t length = 0;
    queue_apply_darg(q->queue, count_arrived, &(q->time_now), &length);
    return length;
}

void req_destroyer(node_t *el, void *plug1, void *plug2)
{
    (void)plug1;
    (void)plug2;
    request_t req = el->data;
    req_destroy(&req);
}

void req_q_reset(req_queue_t q)
{
    q->time_now = 0.0;
    q->length_sum = 0.0;
    q->sleep_time = 0.0;
    q->wait_sum = 0.0;
    q->length_sum = 0.0;
    q->t1_in = 0;
    q->t1_out = 0;
    q->t2_in = 0;
    q->t2_out = 0;

    queue_apply_darg(q->queue, req_destroyer, NULL, NULL);
    queue_clear(q->queue);
}

size_t req_q_get_total_length(const req_queue_t q)
{
    return queue_get_length(q->queue);
}

size_t req_q_get_t1_out(const req_queue_t q)
{
    return q->t1_out;
}

size_t req_q_get_t2_out(const req_queue_t q)
{
    return q->t2_out;
}

double req_q_get_cur_time(const req_queue_t q)
{
    return q->time_now;
}

double req_q_get_sleep_time(const req_queue_t q)
{
    return q->sleep_time;
}
