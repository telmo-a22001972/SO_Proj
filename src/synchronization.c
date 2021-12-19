#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include "synchronization.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


/* Função que cria um novo semáforo com nome name e valor inicial igual a
* value. Pode concatenar o resultado da função getuid() a name, para tornar
* o nome único para o processo.
*/
sem_t * semaphore_create(char* name, int value){
    sem_t *sem = sem_open(name,O_CREAT, 0xFFFFFFFF, value);
    if (sem == SEM_FAILED)
    {
        perror("sem");
        exit(1);
    }
    return sem;
}

/* Função que destroi o semáforo passado em argumento.
*/
void semaphore_destroy(char* name, sem_t* semaphore){
    if (sem_close(semaphore) == -1)
    {
        perror("cona sem_full");
    }
    if (sem_unlink(name)== -1)
    {
        perror("name sem_full");
    }
    
}

/* Função que inicia o processo de produzir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_begin(struct prodcons* pc){
    sem_wait(pc->empty);
    semaphore_mutex_lock(pc->mutex);
}

/* Função que termina o processo de produzir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_end(struct prodcons* pc)
{
    semaphore_mutex_unlock(pc->mutex);
    sem_post(pc->full);
}

/* Função que inicia o processo de consumir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_begin(struct prodcons* pc){ 
    sem_wait(pc->full);
    semaphore_mutex_lock(pc->mutex);
}

/* Função que termina o processo de consumir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_end(struct prodcons* pc){
    semaphore_mutex_unlock(pc->mutex);
    sem_post(pc->empty);

}

/* Função que faz wait a um semáforo.
*/
void semaphore_mutex_lock(sem_t* sem){
    sem_wait(sem);
}

/* Função que faz post a um semáforo.
*/
void semaphore_mutex_unlock(sem_t* sem){
    sem_post(sem);
}
