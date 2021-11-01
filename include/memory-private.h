#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD




//estrutura que representa um buffer circular, completar pelos alunos
struct circular_buffer { 
    int *posicaoLer;
    int *posicaoEscrever;
    struct operation *buffer;
    //int *posicoesEscritas;

};

//estrutura que representa um buffer de acesso aleatório, completar pelos alunos
struct rnd_access_buffer { 	
    struct operation *buffer;
    int *posicaoBuffer;
    
};



#endif
