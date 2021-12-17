#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "log.h"


void registerLog(struct main_data *data, int actionType, int readCounter){
    
    FILE *ficheiro = fopen(data->log_filename, "a+");
    
    struct timespec spec;

    if (ficheiro == NULL)
    {
        puts("Error opening file");
        exit(EXIT_FAILURE);
    }
    else if ( clock_gettime(CLOCK_REALTIME, &spec) == -1)
    {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

    time_t s = spec.tv_sec;
    long ms = (spec.tv_nsec / 1.0e6); //nano para mili
    if (ms>999)
    {
        s++;
        ms=0;
    }
    
    char timeToString[80];
    struct tm *infoTime;
    infoTime = localtime(&s);
    strftime(timeToString,sizeof(timeToString),"%Y-%m-%d %H:%M:%S", infoTime);

    switch (actionType)
    {
    case 0: //op
        fprintf(ficheiro,"%s.%03ld op\n",timeToString,ms);
        break;
    case 1:
        fprintf(ficheiro,"%s.%03ld read %d\n",timeToString,ms,readCounter);
        break;
    
    case 2:
        fprintf(ficheiro,"%s.%03ld stop\n",timeToString,ms);
        break;

    default:
        break;
    }

    fclose(ficheiro);    

}