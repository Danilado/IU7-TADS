#ifndef MY_A_REQ_QUEUE
#define MY_A_REQ_QUEUE

#include "my_a_queue.h"
#include "my_request.h"

#ifndef MAX_WORK_TIME
#define MAX_WORK_TIME 4
#endif

typedef struct req_a_queue *req_queue_a_t;

req_queue_a_t req_a_q_create(void);
void req_a_q_destroy(req_queue_a_t *q);

double req_a_q_get_cur_avg_len(req_queue_a_t q);
double req_a_q_get_cur_avg_wait(req_queue_a_t q);

size_t req_a_q_fill(req_queue_a_t q, size_t t1_count);
enum req_type req_a_q_process_next(req_queue_a_t q);
size_t req_a_q_get_cur_length(const req_queue_a_t q);

void req_a_q_reset(req_queue_a_t q);

size_t req_a_q_get_t1_out(const req_queue_a_t q);
size_t req_a_q_get_t2_out(const req_queue_a_t q);
double req_a_q_get_cur_time(const req_queue_a_t q);
double req_a_q_get_sleep_time(const req_queue_a_t q);

#endif
