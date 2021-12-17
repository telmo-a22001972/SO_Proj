#include <time.h>
#include "main.h"
#include "memory.h"
#include "memory-private.h"


void clock_start_time(struct operation * op_info)
{
    clock_gettime(CLOCK_MONOTONIC, &(op_info->start_time));
}
void clock_client_time(struct operation *op_info)
{
    clock_gettime(CLOCK_MONOTONIC,&(op_info->client_time));
}
void clock_proxy_time(struct operation *op_info)
{
    clock_gettime(CLOCK_MONOTONIC, &(op_info->proxy_time));
}
void clock_server_time(struct operation *op_info)
{
    clock_gettime(CLOCK_MONOTONIC, &(op_info->server_time));
}
void clock_end_time(struct operation *op_info)
{
    clock_gettime(CLOCK_MONOTONIC, &(op_info->end_time));
}