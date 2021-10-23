#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD
#define MAX_OPS 50




//estrutura que representa um buffer circular, completar pelos alunos
struct circular_buffer { 
    

};

//estrutura que representa um buffer de acesso aleatório, completar pelos alunos
struct rnd_access_buffer { 	
    struct operation *buffer[MAX_OPS];
    int posicaoBuffer[MAX_OPS];
    
};

void inicializar_rnd_buffer(struct rnd_access_buffer* buffer);


void ler_bufferstruct (struct rnd_access_buffer* buffer);

void toStringOP(struct operation *op);

#endif
