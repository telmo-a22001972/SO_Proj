#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD
#define MAX_STRINGS 50
#define MAX_LENGHT 258
//Hello




//estrutura que representa um buffer circular, completar pelos alunos
struct circular_buffer { 
    

};

//estrutura que representa um buffer de acesso aleatório, completar pelos alunos
struct rnd_access_buffer { 	
    char *buffer[MAX_STRINGS][MAX_LENGHT];
    int enderecoArray;
    int posicaoBuffer[MAX_STRINGS];
    
};

#endif
