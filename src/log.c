#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

//fazer criar o ficheiro no main??
void registerLog(struct main_data *data, int actionType, int readCounter){
    
    FILE *ficheiro = fopen(data->log_filename, "a+");
    

    if (ficheiro == NULL)
    {
        puts("Error opening file");
        exit(EXIT_FAILURE);
    }

    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );

    switch (actionType)
    {
    case 0: //op

        fprintf(ficheiro, "%d-%d-%d %d:%d:%d op\n", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour,info->tm_min,info->tm_sec);
        
        break;
    case 1:
        fprintf(ficheiro, "%d-%d-%d %d:%d:%d read %d\n", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour,info->tm_min,info->tm_sec, readCounter);
        break;
    
    case 2:
        fprintf(ficheiro, "%d-%d-%d %d:%d:%d stop\n", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour,info->tm_min,info->tm_sec);
        break;

    default:
        break;
    }

    fclose(ficheiro);    

}