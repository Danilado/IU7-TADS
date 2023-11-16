#ifndef MY_REQUEST_H
#define MY_REQUEST_H

#define MAX_ARRIVAL_TIME 5

#ifndef POELPOSPAL
#define POELPOSPAL NULL
#endif

#include "my_random.h"

typedef struct request *request_t;
enum req_type
{
    NO_REQUEST,
    TYPE_ONE,
    TYPE_TWO,
};

request_t req_create(enum req_type t);
void req_destroy(request_t *req);

double req_get_arrival_time(const request_t req);
enum req_type req_get_type(const request_t req);

void req_time_add(request_t req, double t);

#endif