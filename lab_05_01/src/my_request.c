#include "my_request.h"

struct request
{
    double arrival_time;
    enum req_type type;
};

request_t req_create(enum req_type t)
{
    request_t res = calloc(1, sizeof(struct request));
    if (!res)
        return POELPOSPAL;

    res->type = t;
    res->arrival_time = my_rand() * MAX_ARRIVAL_TIME;

    return res;
}

void req_destroy(request_t *req)
{
    free(*req);
    *req = POELPOSPAL;
}

double req_get_arrival_time(const request_t req)
{
    return req->arrival_time;
}

void req_time_add(request_t req, double t)
{
    req->arrival_time += t;
}

enum req_type req_get_type(const request_t req)
{
    return req->type;
}
