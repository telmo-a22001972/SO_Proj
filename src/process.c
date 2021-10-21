#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"



/* Função que inicia um processo através da função fork do SO. O novo
* processo pode ser um cliente, proxy, ou servidor, consoante a variável
* process_code tem valor 0, 1, ou 2 respectivamente, e irá executar a função
* execute_* respetiva, fazendo exit do retorno. O processo pai devolve o pid
* do processo criado.
*/
int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data){
    
}


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int pid = waitpid(process_id);
    return pid;
}