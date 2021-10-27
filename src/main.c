#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char *argv[]) {
    //init data structures
    struct main_data* data = create_dynamic_memory(sizeof(struct
    main_data));
    struct communication_buffers* buffers = 
    create_dynamic_memory(sizeof(struct communication_buffers));
    buffers->main_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    buffers->cli_prx = create_dynamic_memory(sizeof(struct circular_buffer));

    buffers->prx_srv = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    buffers->srv_cli = create_dynamic_memory(sizeof(struct circular_buffer));
    
    
    //execute main code
    main_args(argc, argv, data);
    
    //Inicializar buffers
    inicializar_rnd_buffer(buffers->main_cli, data->buffers_size);
    inicializar_rnd_buffer(buffers->prx_srv, data->buffers_size);
    inicializar_circularbuffer(buffers->cli_prx, data->buffers_size);
    inicializar_circularbuffer(buffers->srv_cli, data->buffers_size);

    struct operation teste;
    struct operation *testeprt;
    testeprt = &teste;
    teste.client = 0;
    teste.id = 0;
    teste.proxy = 0;
    teste.server = 0;
    teste.status = 'C';

    write_rnd_access_buffer(buffers->main_cli, data->buffers_size, testeprt);

    printf("%c\n", buffers->main_cli->buffer[0].status);
    /*
    create_dynamic_memory_buffers(data);
    */
    
    
    /*
    create_shared_memory_buffers(data, buffers);
    
    launch_processes(buffers, data);
    user_interaction(buffers, data);

    //release final memory
    */

    /*
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_cli);
    destroy_dynamic_memory(buffers->cli_prx);
    destroy_dynamic_memory(buffers->prx_srv);
    destroy_dynamic_memory(buffers->srv_cli);
    destroy_dynamic_memory(buffers);
    */
    
}

/* Função que lê os argumentos da aplicação, nomeadamente o número
* máximo de operações, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de clientes, de proxies e de
* servidores. Guarda esta informação nos campos apropriados da
* estrutura main_data.
*/
void main_args(int argc, char* argv[], struct main_data* data){

    if (argc != 6)
    {
        printf("Modo de execução errado.\n");
        exit(1);
    } else {
        data->max_ops = atoi(argv[1]);
        data->buffers_size = atoi(argv[2]);
        data->n_clients = atoi(argv[3]);
        data->n_proxies = atoi(argv[4]);
        data->n_servers = atoi(argv[5]);
        puts("Esketit");
       
    }
    
}

/* Função que reserva a memória dinâmica necessária para a execução
* do socps, nomeadamente para os arrays *_pids e *_stats da estrutura 
* main_data. Para tal, pode ser usada a função create_dynamic_memory.
*/

void create_dynamic_memory_buffers(struct main_data* data){
    data = create_dynamic_memory(sizeof(data));
}

/* Função que reserva a memória partilhada necessária para a execução do
* socps. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication_buffers, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory.
*/

//create_shared_memory vai dar return no pointer da zona de memmória partilhada criada
    //Como é que a data e os buffers vai usar essa zona de memória?
void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){}


/* Função que inicia os processos dos clientes, proxies e
* servidores. Para tal, pode usar a função launch_process,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/

void launch_processes(struct communication_buffers* buffers, struct main_data* data){}

/* Função que faz interação do utilizador, podendo receber 4 comandos:
* op - cria uma nova operação, através da função create_request
* read - verifica o estado de uma operação através da função read_answer
* stop - termina o execução do socps através da função stop_execution
* help - imprime informação sobre os comandos disponiveis
*/

void user_interaction(struct communication_buffers* buffers, struct main_data* data){}

/* Se o limite de operações ainda não tiver sido atingido, cria uma nova
* operação identificada pelo valor atual de op_counter, escrevendo a mesma
* no buffer de memória partilhada entre main e clientes. Imprime o id da
* operação e incrementa o contador de operações op_counter.
*/

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data){}

/* Função que lê um id de operação do utilizador e verifica se a mesma
* é valida e se já foi respondida por um servidor. Em caso afirmativo,
* imprime informação da mesma, nomeadamente o seu estado, e os ids do 
* cliente, proxy e servidor que a processaram.
*/

void read_answer(struct main_data* data){}

/* Função que termina a execução do programa socps. Deve começar por 
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve acordar processos adormecidos, esperar que terminem a sua 
* execução, escrever as estatisticas finais do programa, e por fim libertar
* os semáforos e zonas de memória partilhada e dinâmica previamente 
*reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/

void stop_execution(struct main_data* data, struct communication_buffers* buffers){}

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo clientes, proxies e servidores. Para tal, pode usar a função 
* wait_process do process.h.
*/

void wait_processes(struct main_data* data){}


/* Função que imprime as estatisticas finais do socps, nomeadamente quantas
* operações foram processadas por cada cliente, proxy e servidor.
*/
void write_statistics(struct main_data* data){}

/* Função que liberta todos os buffers de memória dinâmica previamente
* reservados na estrutura data.
*/
void destroy_dynamic_memory_buffers(struct main_data* data){}


/* Função que liberta todos os buffers de memória partilhada previamente
* reservados nas estruturas data e buffers.
*/

void destroy_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){}
