#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include "sotime.h"
#include <signal.h>


/* Função principal de um Proxy. Deve executar um ciclo infinito onde em 
* cada iteração lê uma operação dos clientes e se a mesma tiver id 
* diferente de -1 e se data->terminate ainda for igual a 0, processa-a e
* escreve-a para os servidores. Operações com id igual a -1 são ignoradas
* (op inválida) e se data->terminate for igual a 1 é porque foi dada ordem
* de terminação do programa, portanto deve-se fazer return do número de 
* operações processadas. Para efetuar estes passos, pode usar os outros
* métodos auxiliares definidos em proxy.h.
*/
int execute_proxy(int proxy_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    struct operation op;
    struct operation *op_ptr = &op;
    signal(SIGINT,SIG_IGN);
    while(1){
        
        if (*data->terminate == 1)
        {
            return *data->proxy_stats;
        }
        
        consume_begin(sems->cli_prx);
        proxy_receive_operation(op_ptr, buffers, data, sems);
        consume_end(sems->cli_prx);
        
        if (op_ptr->id != -1 && *data->terminate == 0)
        {
            
            proxy_process_operation(op_ptr, proxy_id, data->proxy_stats);

            produce_end(sems->prx_srv);
            proxy_forward_operation(op_ptr,buffers, data, sems);
            produce_end(sems->prx_srv);
        }

        if (*data->terminate == 1)
        {
            return *data->proxy_stats;
        }
        
        
    }
}


/* Função que lê uma operação do buffer de memória partilhada entre
* clientes e proxies, efetuando a necessária sincronização antes e
* depois de ler. Quando o processo acorda da sincronização, e antes de
* tentar ler a operação, deve verificar se data->terminate tem valor 1.
* Em caso afirmativo, retorna imediatamente da função.
*/
void proxy_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if(*data->terminate == 1) {
        return;
    }
    read_circular_buffer(buffers->cli_prx, data->buffers_size, op);
}
/* Função que processa uma operação, alterando o seu campo proxy para o id
* passado como argumento, alterando o estado da mesma para 'P' (proxied), e 
* incrementando o contador de operações.
*/
void proxy_process_operation(struct operation* op, int server_id, int* counter){
    op->proxy = server_id;
    op->status = 'P';
    *counter+=1;
    //clock proxy processou a op
    clock_proxy_time(op);
}


/* Função que escreve uma operação no buffer de memória partilhada entre
* proxies e servidores, efetuando a necessária sincronização antes e
* depois de escrever.
*/
void proxy_forward_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    write_rnd_access_buffer(buffers->prx_srv, data->buffers_size, op);
}
