#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "configuration.h"
#include <string.h>

void readConfig(struct main_data *data, char file[20]){
    FILE *ficheiro = fopen(file, "r");

    if(ficheiro == NULL)
    {
        puts("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[100];
    char * num = malloc(sizeof(line));

    func_ptr rotinaData[]= {{maxOps},{buffers_size},{n_clients},{n_proxies},{n_servers},{log_filename}};

    

    for(int i = 0; fgets(line, sizeof(line), ficheiro) != NULL ; i++)
    {
      sscanf(line, " %s",num);
      rotinaData[i].func(data,num);
    }
    fclose(ficheiro);
}



//atribui o valor lido de configuração a max_ops
void maxOps (struct main_data *data ,char * num)
{
  data->max_ops = atoi(num);
}
//atribui o valor lido de configuração a buffers_size
void buffers_size (struct main_data *data , char * num)
{
  data->buffers_size = atoi(num);
}
//atribui o valor lido de configuração a n_clients
void n_clients (struct main_data *data , char * num)
{
  data->n_clients = atoi(num);
}
//atribui o valor lido de configuração a n_proxies
void n_proxies (struct main_data *data , char * num)
{
  data->n_proxies = atoi(num);
}
//atribui o valor lido de configuração a n_servers
void n_servers (struct main_data *data , char * num)
{
  data->n_servers = atoi(num);
}
//atribui o valor lido de configuração a log_filename
void log_filename (struct main_data *data , char * num)
{
  data->log_filename = malloc(sizeof(num));
  strcpy(data->log_filename, num);
  free(num);
}