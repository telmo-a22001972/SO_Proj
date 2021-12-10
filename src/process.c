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
#include <string.h>



/* Função que inicia um processo através da função fork do SO. O novo
* processo pode ser um cliente, proxy, ou servidor, consoante a variável
* process_code tem valor 0, 1, ou 2 respectivamente, e irá executar a função
* execute_* respetiva, fazendo exit do retorno. O processo pai devolve o pid
* do processo criado.
*/
int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems)
{
    pid_t pid;
    int status;
    
    pid = fork();
    status = -1;
    if (!pid)
    {
        
        if (process_code==0)
        {
            
            status = execute_client(process_id,buffers,data, sems);
        }
        else if (process_code == 1)
        {
            
            status = execute_proxy(process_id,buffers,data, sems);
        }
        else if(process_code == 2)
        {
            
            status = execute_server(process_id,buffers,data, sems);
        }
    
        exit(status); 
    }

    if (pid != -1)
    {
       
        return pid;
    }
    perror("fork launch");
    exit(1);
    
}


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int status;
    int pid = waitpid(process_id, &status, 0);
    
    if (pid != -1)
    {
        return WEXITSTATUS(status);
    }

    puts("erro wait");
    return 0;
   ;
    

    
}