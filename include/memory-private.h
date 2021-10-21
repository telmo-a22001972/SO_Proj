#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD
#define MAX 50


//estrutura que representa um buffer circular, completar pelos alunos
struct circular_buffer { 	
    char tamanho[MAX][MAX];
    int posicaoEscrever;
    int posicaoLer;

};

//estrutura que representa um buffer de acesso aleatório, completar pelos alunos
struct rnd_access_buffer { 	
    
    int enderecoArray;
    int posicaoBuffer[MAX];
    
};

#endif
