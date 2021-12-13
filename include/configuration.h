
typedef struct {
    void (*func)();
} func_ptr;

//le ficheiro de configuração
void readConfig(struct main_data *data, char file[20]);
//atribui o valor lido de configuração a max_ops
void maxOps (struct main_data *data , char * num);
//atribui o valor lido de configuração a buffers_size
void buffers_size (struct main_data *data , char * num);
//atribui o valor lido de configuração a n_clients
void n_clients (struct main_data *data , char * num);
//atribui o valor lido de configuração a n_proxies
void n_proxies (struct main_data *data , char * num);
//atribui o valor lido de configuração a n_servers
void n_servers (struct main_data *data , char * num);
//atribui o valor lido de configuração a log_filename
void log_filename (struct main_data *data , char * num);

