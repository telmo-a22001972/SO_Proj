#ifndef CLIENT_H_GUARD
#define CLIENT_H_GUARD

#include "memory.h"
#include "main.h"

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
int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data);


/* Função que lê uma operação do buffer de memória partilhada entre a 
* main e clientes. Antes de tentar ler a operação, deve verificar se 
* data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente 
* da função.
*/
void client_get_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data);


/* Função que processa uma operação, alterando o seu campo cliente para o id
* passado como argumento, alterando o estado da mesma para 'C' (client), e 
* incrementando o contador de operações.
*/
void client_process_operation(struct operation* op, int cient_id, int* counter);


/* Função que escreve uma operação no buffer de memória partilhada entre
* clientes e proxies.
*/
void client_send_operation(struct operation* op, struct communication_buffers* buffers, struct main_data* data);


/* Função que lê uma operação do buffer de memória partilhada entre 
* servidores e clientes. Antes de tentar ler a operação, deve verificar 
* se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente 
* da função.
*/
void client_receive_answer(struct operation* op, struct communication_buffers* buffers, struct main_data* data);


/* Função que guarda uma operação no array de operações finalizadas da
* estrutura data, usando o id da mesma para indexar o array.
*/
void client_process_answer(struct operation* op, struct main_data* data);

#endif
