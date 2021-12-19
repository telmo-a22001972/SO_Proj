#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include "sotime.h"
#include "synchronization.h"


#include "log.h"
#include "configuration.h"

struct main_data *data;
struct communication_buffers *buffers;
struct semaphores *sems;

void ctrlC() {

   stop_execution(data,buffers,sems);

   exit(0);
}


int main(int argc, char *argv[])
{
    
    //init data structures
    data = create_dynamic_memory(sizeof(struct main_data));

    buffers = create_dynamic_memory(sizeof(struct communication_buffers));

    sems = create_dynamic_memory(sizeof(struct semaphores));

    buffers->main_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    buffers->cli_prx = create_dynamic_memory(sizeof(struct circular_buffer));

    buffers->prx_srv = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    buffers->srv_cli = create_dynamic_memory(sizeof(struct circular_buffer));
    
    sems->main_cli = create_dynamic_memory(sizeof(struct prodcons));
    
    sems->cli_prx = create_dynamic_memory(sizeof(struct prodcons));
    
    sems->prx_srv = create_dynamic_memory(sizeof(struct prodcons));
    
    sems->srv_cli = create_dynamic_memory(sizeof(struct prodcons));

    data->log_filename = create_dynamic_memory(sizeof(char[20]));

    
    signal(SIGINT, ctrlC);
    
    
    
    
    //execute main codeS        
    
    main_args(argc, argv, data);

    create_dynamic_memory_buffers(data);
    
    create_shared_memory_buffers(data, buffers);
//criacao de semaforos
    create_semaphores(data, sems);
    
    launch_processes(buffers, data, sems);
    
    user_interaction(buffers, data, sems);
    
}

/* Função que lê os argumentos da aplicação, nomeadamente o número
* máximo de operações, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de clientes, de proxies e de
* servidores. Guarda esta informação nos campos apropriados da
* estrutura main_data.
*/
void main_args(int argc, char *argv[], struct main_data *data)
{

    if (argc != 2)
    {
        printf("Modo de execução errado.\n");
        puts("Exemplo: ./sovac config.txt");
        exit(1);
    }
    else
    {
        readConfig(data, argv[1]);
        
    }
}

/* Função que reserva a memória dinâmica necessária para a execução
* do socps, nomeadamente para os arrays *_pids e *_stats da estrutura 
* main_data. Para tal, pode ser usada a função create_dynamic_memory.
*/

void create_dynamic_memory_buffers(struct main_data *data)
{
    //Alocar memória para cada array de pids da data, do tamanho n_clientes, n_proxies, n_servers
    data->client_pids = create_dynamic_memory(data->n_clients * sizeof(data->client_pids));
    
    data->proxy_pids = create_dynamic_memory(data->n_proxies * sizeof(data->proxy_pids));
    data->server_pids = create_dynamic_memory(data->n_servers * sizeof(data->server_pids));

    //Alocar memória para cada array de status da ta, do tamanho n_clientes, n_proxies, n_servers
    data->client_stats = create_dynamic_memory(data->n_clients * sizeof(data->client_stats));
    for (int i = 0; i < data->n_clients ; i++)
    {
        data->client_stats[i] = 0;
    }
    
    data->proxy_stats = create_dynamic_memory(data->n_proxies * sizeof(data->proxy_stats));
    for (int i = 0; i < data->n_proxies; i++)
    {
        data->proxy_stats[i] = 0;
    }
    data->server_stats = create_dynamic_memory(data->n_servers * sizeof(data->server_stats));
    for (int i = 0; i < data->n_servers; i++)
    {
        data->server_stats[i] = 0;
    }
    
}

/* Função que reserva a memória partilhada necessária para a execução do
* socps. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication_buffers, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory.
*/
void create_shared_memory_buffers(struct main_data *data, struct communication_buffers *buffers)
{

    //Alocamos meḿoria partilhada apenas para o buffer cli_prx
    buffers->cli_prx->buffer = create_shared_memory("/cli_prx_buffer", data->max_ops * sizeof(buffers->cli_prx->buffer));
    //buffers->cli_prx->posicoesEscritas = create_shared_memory("/cli_prx_int_arr", data->max_ops*sizeof(int*));
    buffers->cli_prx->posicaoEscrever = create_shared_memory("/cli_prx_write_pos", sizeof(buffers->cli_prx->posicaoEscrever));
    *buffers->cli_prx->posicaoEscrever = 0;
    buffers->cli_prx->posicaoLer = create_shared_memory("/cli_prx_read_pos", sizeof(buffers->cli_prx->posicaoLer));
    *buffers->cli_prx->posicaoLer = 0;

    //Alocamos memória partilhada apenas no buffer main_cli
    buffers->main_cli->buffer = create_shared_memory("/main_cli_buffer", data->max_ops * sizeof(buffers->main_cli->buffer));
    buffers->main_cli->posicaoBuffer = create_shared_memory("/main_cli_int_arr", data->max_ops * sizeof(buffers->main_cli->posicaoBuffer));

    //Alocamos memória partilhada para o buffer prx_srv
    buffers->prx_srv->buffer = create_shared_memory("/prx_srv_buffer", data->max_ops * sizeof(buffers->prx_srv->buffer));
    buffers->prx_srv->posicaoBuffer = create_shared_memory("/prx_srv_int_arr", data->max_ops * sizeof(buffers->prx_srv->posicaoBuffer));

    //Alocamos memória partilhada para o buffer srv_cli
    buffers->srv_cli->buffer = create_shared_memory("/srv_cli_buffer", data->max_ops * sizeof(buffers->srv_cli->buffer));
    //buffers->srv_cli->posicoesEscritas = create_shared_memory("/srv_cli_int_arr", data->max_ops*sizeof(int*));
    buffers->srv_cli->posicaoEscrever = create_shared_memory("/srv_cli_write_pos", sizeof(int));
    *buffers->srv_cli->posicaoEscrever = 0;
    buffers->srv_cli->posicaoLer = create_shared_memory("/srv_cli_read_pos", sizeof(int));
    *buffers->srv_cli->posicaoLer = 0;

    //Alocamos memória partilhada para o array results
    data->results = create_shared_memory("/data_results", data->max_ops * sizeof(data->results));
    data->terminate = create_shared_memory("/data_terminate", sizeof(int));
}

/* Função que inicializa os semáforos da estrutura semaphores. Semáforos
* *_full devem ser inicializados com valor 0, semáforos *_empty com valor
* igual ao tamanho dos buffers de memória partilhada, e os *_mutex com valor
* igual a 1. Para tal pode ser usada a função semaphore_create.
*/
void create_semaphores(struct main_data* data, struct semaphores* sems)
{
    
    sems->main_cli->full = semaphore_create("/sem_main_cli_full", 0);
    sems->main_cli->empty = semaphore_create( "/sem_main_cli_empty", data->buffers_size);
    sems->main_cli->mutex = semaphore_create( "/sem_main_cli_mutex", 1);
    
    sems->cli_prx->full = semaphore_create( "/sem_cli_prx_full", 0);
    sems->cli_prx->empty = semaphore_create( "/sem_cli_prx_empty", data->buffers_size);
    sems->cli_prx->mutex = semaphore_create( "/sem_cli_prx_mutex", 1);
 
    sems->prx_srv->full = semaphore_create( "/sem_prx_srv_full", 0);
    sems->prx_srv->empty = semaphore_create( "/sem_prx_srv_empty", data->buffers_size);
    sems->prx_srv->mutex = semaphore_create( "/sem_prx_srv_mutex", 1);
    
    sems->srv_cli->full = semaphore_create( "/sem_srv_cli_full", 0);
    sems->srv_cli->empty = semaphore_create( "/sem_srv_cli_empty", data->buffers_size);
    sems->srv_cli->mutex = semaphore_create( "/sem_srv_cli_mutex", 1);

    sems->results_mutex = semaphore_create("/sem_results_mutex",1);
   
}

/* Função que inicia os processos dos clientes, proxies e
* servidores. Para tal, pode usar a função launch_process,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/

void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems)
{
    
    //Launch clients, process_id = 0
    int i;
    for (i = 0; i < data->n_clients; i++)
    {
        
        data->client_pids[i]=launch_process(i, 0, buffers, data, sems);
        
    }

    //launch proxies, procces_id = 1
    for (i = 0; i < data->n_proxies; i++)
    {

        data->proxy_pids[i] = launch_process(i, 1, buffers, data, sems);
        
    }

    //launch servers, process_id= 2
    for (i = 0; i < data->n_servers; i++)
    {
        
        data->server_pids[i]=launch_process(i, 2, buffers, data, sems);
        
    }
}

/* Função que faz interação do utilizador, podendo receber 4 comandos:
* op - cria uma nova operação, através da função create_request
* read - verifica o estado de uma operação através da função read_answer
* stop - termina o execução do sovac através da função stop_execution
* help - imprime informação sobre os comandos disponiveis
*/
void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems)
{
    char menuOp[32];
    
    int *op_counter_ptr = create_dynamic_memory(sizeof(int));
    *op_counter_ptr = 0;

    do
    {
        printf("enter command:\n");
        scanf("%s", menuOp);
        if (strcmp(menuOp, "op") == 0)
        {
            /*criar operacao*/
            
            create_request(op_counter_ptr, buffers, data, sems);
        }
        else if (strcmp(menuOp, "read") == 0)
        {
            read_answer(data, sems);
        }
        else if (strcmp(menuOp, "help") == 0)
        {
            printf("Available commands:\n\top - creates a new request\n\tread x - attempts to read the response to request x\n\tstop - stops the execution of this program.\n\thelp - prints these instructions.");
        }
        else if(strcmp(menuOp, "stop") == 0){
            //chamar a função stop_execution
            registerLog(data, 2, 0);
            stop_execution(data, buffers,sems);
        }else
        {
            printf("Command not recognized, type \'help\' for assistance.\n");
        }

        
    } while (strcmp(menuOp, "stop"));
    destroy_dynamic_memory(op_counter_ptr);
}

/* Se o limite de operações ainda não tiver sido atingido, cria uma nova
* operação identificada pelo valor atual de op_counter, escrevendo a mesma
* no buffer de memória partilhada entre main e clientes e efetuando a 
* necessária sincronização antes e depois de escrever. Imprime o id da
* operação e incrementa o contador de operações op_counter.
*/

void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    
    
    
    if (*op_counter >= data->max_ops)
    {
        puts("max ops has been reached!");
        return;
    }else{
        registerLog(data, 0, 0);
        struct operation *op_ptr;
        //clock op init
        clock_start_time(op_ptr);
        op_ptr->id = *op_counter;
        produce_begin(sems->main_cli);
        write_rnd_access_buffer(buffers->main_cli, data->buffers_size, op_ptr);
        produce_end(sems->main_cli);
        printf("operation %d created!\n", *op_counter);

        *op_counter+=1;
        
    }

}

/* Função que lê um id de operação do utilizador e verifica se a mesma
* é valida e se já foi respondida por um servidor. Em caso afirmativo,
* imprime informação da mesma, nomeadamente o seu estado, e os ids do 
* cliente, proxy e servidor que a processaram. O acesso à estrutura 
* data->results deve ser sincronizado com as funções e semáforos
* respetivos.
*/
void read_answer(struct main_data* data, struct semaphores* sems) {
    int read , i;
    struct operation * opPtr = malloc(sizeof(struct operation));

    scanf(" %d", &read);
    
    if (read >= data->max_ops || read < 0)
    {
        puts("op id provided is invalid!");
    }
    else{
        *opPtr = data->results[read];
        if (opPtr->status == 'S')
        {
            //registar opcao no log
            registerLog(data, 1, read);
            printf("op %d with status %c was received by client %d, forwarded by proxy %d, and served by server %d\n", opPtr->id , opPtr->status , opPtr->client, opPtr->proxy , opPtr->server);
        }
        else {
            
            printf("op %d is not yet available!\n", read);
        }
        
    } 
    destroy_dynamic_memory(opPtr);
}

/* Função que termina a execução do programa sovac. Deve começar por 
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve acordar processos adormecidos, esperar que terminem a sua 
* execução, escrever as estatisticas finais do programa, e por fim libertar
* os semáforos e zonas de memória partilhada e dinâmica previamente 
*reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems) {
    
    *data->terminate = 1;
    wakeup_processes(data, sems);
    
    wait_processes(data);
    
    destroy_semaphores(sems);
    write_statistics(data);

    /*destruir memória*/
    
    destroy_shared_memory_buffers(data, buffers);
    destroy_dynamic_memory_buffers(data);
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_cli);
    destroy_dynamic_memory(buffers->cli_prx);
    destroy_dynamic_memory(buffers->prx_srv);
    destroy_dynamic_memory(buffers->srv_cli);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(data->log_filename);
}

/* Função que acorda todos os processos adormecidos em semáforos, para que
* estes percebam que foi dada ordem de terminação do programa. Para tal,
* pode ser usada a função produce_end sobre todos os conjuntos de semáforos
* onde possam estar processos adormecidos e um número de vezes igual ao 
* máximo de processos que possam lá estar.
*/
void wakeup_processes(struct main_data* data, struct semaphores* sems){
    int num_processos;
    if (data->n_clients>=data->n_proxies && data->n_clients>=data->n_servers)
    {
        num_processos = data->n_clients;

    } else if (data->n_proxies >= data->n_servers && data->n_proxies >= data->n_clients)
    {
        num_processos = data->n_proxies;

    } else
    {
        num_processos = data->n_servers;
    }
    
    for (int i = 0; i < num_processos; i++)
    {
        produce_end(sems->main_cli);
        produce_end(sems->cli_prx);
        produce_end(sems->prx_srv);
        produce_end(sems->srv_cli);
        
    }

    
    
}


/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo clientes, proxies e servidores. Para tal, pode usar a função 
* wait_process do process.h.
*/

void wait_processes(struct main_data *data) {
    int i;
    for (i = 0; i < data->n_clients; i++)
    {   
        data->client_stats[i] = wait_process(data->client_pids[i]);
    }

    for (i = 0; i < data->n_proxies; i++)
    {
        data->proxy_stats[i] = wait_process(data->proxy_pids[i]);
    }

    for (i = 0; i < data->n_servers; i++)
    {

        data->server_stats[i] = wait_process(data->server_pids[i]);
    }

    return;
    
}

/* Função que imprime as estatisticas finais do socps, nomeadamente quantas
* operações foram processadas por cada cliente, proxy e servidor.
*/
void write_statistics(struct main_data *data) {
    int i;
    printf("\n");
    for (i = 0; i < data->n_clients; i++)
    {   
        printf("O client %d processou %d operações\n", i, data->client_stats[i]);
        
    }

    for (i = 0; i < data->n_proxies; i++)
    {   
        printf("O proxy %d processou %d operações\n", i, data->proxy_stats[i]);
        
    }
    

    for (i = 0; i < data->n_servers; i++)
    {   
        printf("O server %d processou %d operações\n", i, data->server_stats[i]);
    }
}

/* Função que liberta todos os buffers de memória dinâmica previamente
* reservados na estrutura data.
*/
void destroy_dynamic_memory_buffers(struct main_data *data)
{
    free(data->client_pids);
    free(data->proxy_pids);
    free(data->server_pids);

    free(data->client_stats);
    free(data->proxy_stats);
    free(data->server_stats);
    
}

/* Função que liberta todos os buffers de memória partilhada previamente
* reservados nas estruturas data e buffers.
*/
void destroy_shared_memory_buffers(struct main_data *data, struct communication_buffers *buffers)
{

    
    //Free no buffer main_cli
    destroy_shared_memory("/main_cli_buffer", buffers->main_cli->buffer, data->max_ops * sizeof(buffers->main_cli->buffer));
    destroy_shared_memory("/main_cli_int_arr", buffers->main_cli->posicaoBuffer, data->max_ops * sizeof(buffers->main_cli->posicaoBuffer));

    //Free no buffer cli_prx
    destroy_shared_memory("/cli_prx_buffer", buffers->cli_prx->buffer, data->max_ops * sizeof(buffers->cli_prx->buffer));
    destroy_shared_memory("/cli_prx_write_pos", buffers->cli_prx->posicaoEscrever, sizeof(buffers->cli_prx->posicaoEscrever));
    destroy_shared_memory("/cli_prx_read_pos", buffers->cli_prx->posicaoLer, sizeof(buffers->cli_prx->posicaoLer));

    //Free no buffer srv_cli
    destroy_shared_memory("/srv_cli_buffer", buffers->srv_cli->buffer, data->max_ops * sizeof(buffers->cli_prx->buffer));
    destroy_shared_memory("/srv_cli_write_pos", buffers->srv_cli->posicaoEscrever, sizeof(int));
    destroy_shared_memory("/srv_cli_read_pos", buffers->srv_cli->posicaoLer, sizeof(int));

    //Free no buffer prx_srv
    destroy_shared_memory("/prx_srv_buffer", buffers->prx_srv->buffer, data->max_ops * sizeof(buffers->main_cli->buffer));
    destroy_shared_memory("/prx_srv_int_arr", buffers->prx_srv->posicaoBuffer, data->max_ops * sizeof(buffers->main_cli->posicaoBuffer));

    //Free no array results e no int terminate
    destroy_shared_memory("/data_results", data->results, data->max_ops * sizeof(data->results));
    destroy_shared_memory("/data_terminate", data->terminate, sizeof(int));

    
}

/* Função que liberta todos os semáforos da estrutura semaphores.
*/
void destroy_semaphores(struct semaphores* sems){
    semaphore_destroy("/sem_main_cli_full", sems->main_cli->full);
    semaphore_destroy("/sem_main_cli_empty",sems->main_cli->empty);
    semaphore_destroy("/sem_main_cli_mutex",sems->main_cli->mutex);

    semaphore_destroy("/sem_cli_prx_full",sems->cli_prx->full);
    semaphore_destroy("/sem_cli_prx_empty",sems->cli_prx->empty);
    semaphore_destroy("/sem_cli_prx_mutex",sems->cli_prx->mutex);

    semaphore_destroy("/sem_prx_srv_full",sems->prx_srv->full);
    semaphore_destroy("/sem_prx_srv_empty",sems->prx_srv->empty);
    semaphore_destroy("/sem_prx_srv_mutex",sems->prx_srv->mutex);
 
    semaphore_destroy("/sem_srv_cli_full",sems->srv_cli->full);
    semaphore_destroy("/sem_srv_cli_empty",sems->srv_cli->empty);
    semaphore_destroy("/sem_srv_cli_mutex",sems->srv_cli->mutex);

    semaphore_destroy("/sem_results_mutex" , sems->results_mutex);
    
    destroy_dynamic_memory(sems->main_cli);
    destroy_dynamic_memory(sems->cli_prx);
    destroy_dynamic_memory(sems->prx_srv);
    destroy_dynamic_memory(sems->srv_cli);
    
    //semaphore_destroy("/sem_results_mutex",sems);
}
