#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include <sys/wait.h>
#include <unistd.h>




/* Função que inicia um processo através da função fork do SO. O novo
* processo pode ser um cliente, proxy, ou servidor, consoante a variável
* process_code tem valor 0, 1, ou 2 respectivamente, e irá executar a função
* execute_* respetiva, fazendo exit do retorno. O processo pai devolve o pid
* do processo criado.
*/
int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data){
    int pid;
    switch (process_code)
    {
        case 0:
            
            pid = fork();
            if (pid == -1)
            {
                puts("Erro fork");
                exit(1);
            }
            if(pid == 0){
                int pid = getpid();
                data->client_pids[process_id] = pid;
                execute_client(process_id, buffers,data);
                
            }

            
            break;

        case 1:
            pid = fork();
            if (pid == -1)
            {
               puts("Erro fork");
               exit(1);
            }
            if (pid == 0)
            {
                int pid = getpid();
                data->proxy_pids[process_id] = pid;
                execute_proxy(process_id, buffers, data);
            }
            break;
        
        case 2:
            pid = fork();
            if (pid == -1)
            {
               puts("Erro fork");
               exit(1);
            }
            if (pid == 0)
            {
                int pid = getpid();
                data->server_pids[process_id] = pid;
                execute_server(process_id, buffers, data);
            }
            break;     
    default:
        break;
    }
    
    return pid;
    
}


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int status;
    int pid = waitpid(process_id, &status,0);
    
    //Terminou corretamente
    if (WIFEXITED(status) != 0)
    {
        return WIFEXITED(status);
    }

    //Não terminou correamente
    return 0;
}