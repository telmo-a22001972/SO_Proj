#include <time.h>
#include "memory.h"


void clock_start_time(struct operation * op_info);
void clock_client_time(struct operation * op_info);
void clock_proxy_time(struct operation * op_info);
void clock_server_time(struct operation * op_info);
void clock_end_time(struct operation * op_info);