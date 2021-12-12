#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include <string.h>

void readConfig(struct main_data *data, char file[20]){
    FILE *ficheiro = fopen(file, "r");

    if(ficheiro == NULL)
    {
        puts("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[100];
    char op[100];
    char num[100];

    while(fgets(line, sizeof(line), ficheiro) != NULL)
    {
        sscanf(line,"%s %s", op, num);
                
        if (strcmp(op, "max_ops") == 0)
        {
            data->max_ops = atoi(num);
        } 
        if (strcmp(op, "buffers_size") == 0)
        {
            data->buffers_size = atoi(num);
        }
        if (strcmp(op, "n_clients") == 0)
        {
            data->n_clients = atoi(num);
        }
        if (strcmp(op, "n_proxies") == 0)
        {
            data->n_proxies = atoi(num);
        }
        if (strcmp(op, "n_servers") == 0)
        {
            data->n_servers = atoi(num);
        }
        if (strcmp(op, "log_filename") == 0)
        {
            
            strcpy(data->log_filename, num);
            break;
        }
        
    }
    
    fclose(ficheiro);
    
    
}