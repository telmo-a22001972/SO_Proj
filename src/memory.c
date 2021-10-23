#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h>


/* Função que reserva uma zona de memória partilhada com tamanho indicado
* por size e nome name, preenche essa zona de memória com o valor 0, e 
* retorna um apontador para a mesma. Pode concatenar o resultado da função
* getuid() a name, para tornar o nome único para o processo.
*/
void* create_shared_memory(char* name, int size){
    

    int *ptr;
    int ret;
    int fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror(name);
        exit(1);
    }

    ret = ftruncate(fd, sizeof(size));
    if (ret == -1){
        perror(name);
        exit(2);
    }

    ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED){
        perror("error-mmap");
        exit(3);
    }
    
    return ptr;
}


/* Função que reserva uma zona de memória dinâmica com tamanho indicado
* por size, preenche essa zona de memória com o valor 0, e retorna um 
* apontador para a mesma.
*/
void* create_dynamic_memory(int size){
    malloc(sizeof(size));
}


/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size){
    int ret;

    ret = munmap(ptr, sizeof(size));
    if (ret == -1){
    perror(name);
    exit(7);
    }
    ret = shm_unlink(name);
    if (ret == -1){
    perror(name);
    exit(8);
    }
    exit(0);
}


/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_dynamic_memory(void* ptr){
    free(ptr);
}


/* Função que escreve uma operação num buffer de acesso aleatório. A
* operação deve ser escrita numa posição livre do buffer, segundo as
* regras de escrita em buffers de acesso aleatório. Se não houver nenhuma
* posição livre, não escreve nada.
*/

void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int i;
    for (i = 0; i < sizeof(buffer->posicaoBuffer); i++)
    {
        if (buffer->posicaoBuffer[i] == 0)
        {
            buffer->buffer[i] = op;
            buffer->posicaoBuffer[i] = 1;
            printf("Escrito na posicao %d\n", i);
            break;
        }
        
    }
    
    
}


/* Função que escreve uma operação num buffer circular. A operação deve 
* ser escrita numa posição livre do buffer, segundo as regras de escrita
* em buffers circulares. Se não houver nenhuma posição livre, não escreve
* nada.
*/

void write_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){}


/* Função que lê uma operação de um buffer de acesso aleatório, se houver
* alguma disponível para ler. A leitura deve ser feita segundo as regras
* de leitura em buffers acesso aleatório. Se não houver nenhuma operação
* disponível, afeta op com o valor -1.
*/
void read_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){}


/* Função que lê uma operação de um buffer circular, se houver alguma 
* disponível para ler. A leitura deve ser feita segundo as regras de
* leitura em buffers circular. Se não houver nenhuma operação disponível,
* afeta op->id com o valor -1.
*/
void read_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){}
