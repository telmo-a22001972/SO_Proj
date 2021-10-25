#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"

void inicializar_rnd_buffer(struct rnd_access_buffer* buffer){
    int i;
    for (i = 0; i < sizeof(buffer->posicaoBuffer); i++)
    {
        buffer->posicaoBuffer[i] = 0;
    }
    
}

void ler_bufferstruct (struct rnd_access_buffer* buffer){
    
    int i;
    for (i = 0; i < sizeof(buffer->posicaoBuffer); i++)
    {
        if (buffer->posicaoBuffer[i] != 0)
        {
           toStringOP(buffer->buffer[i]);
        }
        
        
    }
}

void toStringOP(struct operation *op){
    printf("id: %d, status: %c, client: %d, proxy: %d, server: %d\n", op->id, op->status, op->client, op->proxy, op->server);
}

void inicializar_circularbuffer(struct circular_buffer *buffer) {
    int i;
    buffer->posicaoEscrever = 0;
    buffer->posicaoLer = 0;
    for (i = 0; i < sizeof(buffer->posicoesEscritas); i++)
    {
        buffer->posicoesEscritas[i] = 0;
    }
    
}