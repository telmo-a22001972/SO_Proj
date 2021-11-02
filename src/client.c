#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"


/* Função principal de um Cliente. Deve executar um ciclo infinito
* onde em cada iteração tem dois passos: primeiro, lê uma operação
* da main e se a mesma lida tiver id diferente de -1 e se data->terminate
* ainda for igual a 0, processa-a e escreve-a para os proxies; segundo,
* lê uma resposta dos servidores e se a mesma tiver id diferente de -1 e se
* data->terminate ainda for igual a 0, processa-a. Operações com id igual a
* -1 são ignoradas (op inválida) e se data->terminate for igual a 1 é porque
* foi dada ordem de terminação do programa, portanto deve-se fazer return do
* o número de operações processadas. Para efetuar estes passos, pode usar os
* outros métodos auxiliares definidos em client.h. 
*/
int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data){
    struct operation op;
    struct operation *op_ptr = &op;
    while (1)
    {
        client_get_operation(op_ptr, buffers, data);

        if (op_ptr->id != 1 && data->terminate == 0)
        {
            //Nao percebo esse counter que se tem de passar.
            client_process_operation(op_ptr, client_id, data->n_clients);
            client_send_operation(op_ptr, buffers, data);
        }


        //O que acontece aqui pelo meio
        client_receive_answer(op_ptr, buffers, data);
        if (op_ptr->id != 1 && data->terminate == 0)
        {
            //client_process_operation(op_ptr, client_id, /*e agora*/);
        }
        
        
    }
    
}


/* Função que lê uma operação do buffer de memória partilhada entre a 
* main e clientes. Antes de tentar ler a operação, deve verificar se 
* data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente 
* da função.
*/
void client_get_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    if (*data->terminate == 1)
    {
        //Toca a desligar
        return;
    }
    else {
        read_rnd_access_buffer(buffers->main_cli, data->buffers_size, op);
    }
    
}


/* Função que processa uma operação, alterando o seu campo cliente para o id
* passado como argumento, alterando o estado da mesma para 'C' (client), e 
* incrementando o contador de operações.
*/
void client_process_operation(struct operation* op, int cient_id, int* counter){
    op->id = cient_id;
    op->status = 'C';
    *counter +=1;
}


/* Função que escreve uma operação no buffer de memória partilhada entre
* clientes e proxies.
*/
void client_send_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    write_circular_buffer(buffers->cli_prx, data->buffers_size, op);
}


/* Função que lê uma operação do buffer de memória partilhada entre 
* servidores e clientes. Antes de tentar ler a operação, deve verificar 
* se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente 
* da função.
*/
void client_receive_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data){
    if (data->terminate == 1)
    {
        //toca a bazar
        return;
    }
    
    read_circular_buffer(buffers->srv_cli, data->buffers_size, op);
}


/* Função que guarda uma operação no array de operações finalizadas da
* estrutura data, usando o id da mesma para indexar o array.
*/

void client_process_answer(struct operation* op, struct main_data* data){
    data->results[op->id] = *op;
}
