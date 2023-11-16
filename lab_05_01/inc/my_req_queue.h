#ifndef MY_REQ_QUEUE
#define MY_REQ_QUEUE

#include "my_queue.h"
#include "my_request.h"

typedef struct req_queue *req_queue_t;

req_queue_t req_q_create(void);
void req_q_destroy(req_queue_t *q);

double req_q_get_cur_avg_len(req_queue_t q);
double req_q_get_cur_avg_wait(req_queue_t q);

size_t req_q_fill(req_queue_t q, size_t t1_count);
enum req_type req_q_process_next(req_queue_t q);
size_t req_q_get_cur_length(const req_queue_t q);
size_t req_q_get_total_length(const req_queue_t q);

void req_q_reset(req_queue_t q);

#endif
