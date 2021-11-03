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
                /*Na descrição da função diz, que o filho vai fazer
                * exit do retorno, este retorno é o que o execute_client dá return?
                * ou seja o client_stats
                * E então nese caso fazemos int execute_return = execute_client(...) e depois
                * no fim damos exit(execute_return)?
                * Então e se fizermos isto como é que sabemos que terminou corretamente?
                * porque o normal é se terminou bem, usa-se o exit(0).
                */

                int execute_return;
                
                execute_client(process_id, buffers,data);
                execute_return = execute_client(process_id, buffers,data);
                exit(execute_return);
                //exit(0);
                
            }else{
                int status;
                //Receber o pid do filho por um pipe?
                
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
                
                execute_proxy(process_id, buffers, data);
                exit(0);
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
                
                execute_server(process_id, buffers, data);
                exit(0);
            }
        break;    

        default:

        break;
    }
    

    if (process_code == 0)
    {
        return data->client_pids[process_id];
    }
    
    return pid;
    
}


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int status;
    int pid = waitpid(process_id, &status, 0);
    
    //Terminou corretamente
    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    //Não terminou correamente
    return 0;
}