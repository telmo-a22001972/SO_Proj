#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"


/* Função principal de um Proxy. Deve executar um ciclo infinito onde em 
* cada iteração do ciclo lê uma operação dos clientes e se a mesma tiver id 
* diferente de -1 e se data->terminate ainda for igual a 0, processa-a e
* escreve-a para os servidores. Operações com id igual a -1 são ignoradas
* (op inválida) e se data->terminate for igual a 1 é porque foi dada ordem
* de terminação do programa, portanto deve-se fazer return do número de 
* operações processadas. Para efetuar estes passos, pode usar os outros
* métodos auxiliares definidos em proxy.h.
*/
int execute_proxy(int proxy_id, struct communication_buffers* buffers, struct main_data* data){
    struct operation op;
    struct operation *op_ptr = &op;

    while(1){

        proxy_receive_operation(op_ptr, buffers, data);
        if (op_ptr->id != 1 && *data->terminate == 0)
        {
            proxy_process_operation(op_ptr, proxy_id, data->proxy_stats);
            proxy_forward_operation(op_ptr,buffers, data);
        }

        if (*data->terminate = 1)
        {
            return *data->proxy_stats;
        }
        
        
    }
}


/* Função que lê uma operação do buffer de memória partilhada entre
* clientes e proxies. Antes de tentar ler a operação, deve verificar 
* se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente 
* da função.
*/
void proxy_receive_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    if(*data->terminate == 1) {
        //baza
        return;
    }
    read_circular_buffer(buffers->cli_prx, data->buffers_size, op);
}
/* Função que processa uma operação, alterando o seu campo proxy para o id
* passado como argumento, alterando o estado da mesma para 'P' (proxied), e 
* incrementando o contador de operações.
*/
void proxy_process_operation(struct operation* op, int server_id, int* counter){
    //Perguntar ao naercio
    op->proxy = server_id;
    op->status = 'P';
    *counter+=1;
}


/* Função que escreve uma operação no buffer de memória partilhada entre
* proxies e servidores.
*/
void proxy_forward_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    write_rnd_access_buffer(buffers->prx_srv, data->buffers_size, op);
}
