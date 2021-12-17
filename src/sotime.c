#include <time.h>
#include "main.h"
#include "memory.h"
#include "memory-private.h"


void clock_start_time(struct operation * op_info)
{
    op_info->start_time = create_dynamic_memory(sizeof(op_info->start_time));
    clock_gettime(CLOCK_MONOTONIC, op_info->start_time);
}
void clock_client_time(struct operation *op_info)
{
    op_info->client_time = create_dynamic_memory(sizeof(op_info->client_time));
    clock_gettime(CLOCK_MONOTONIC,op_info->client_time);
}
void clock_proxy_time(struct operation *op_info)
{
    op_info->proxy_time = create_dynamic_memory(sizeof(op_info->client_time));
    clock_gettime(CLOCK_MONOTONIC, op_info->proxy_time);
}
void clock_server_time(struct operation *op_info)
{
    op_info->server_time = create_dynamic_memory(sizeof(op_info->server_time));
    clock_gettime(CLOCK_MONOTONIC, op_info->server_time);
}
void clock_end_time(struct operation *op_info)
{
    op_info->end_time = create_dynamic_memory(sizeof(op_info->end_time));
    clock_gettime(CLOCK_MONOTONIC, op_info->end_time);
}