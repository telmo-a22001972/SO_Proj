#include <stdio.h>
#include "client.h"
#include "main.h"
#include "memory-private.h"
#include "memory.h"
#include "process.h"
#include "proxy.h"
#include "server.h"
#include "memory.h"
#include "stdlib.h"

void inicializar_rnd_buffer(struct rnd_access_buffer* buffer, int buffer_size){
    buffer->buffer = create_dynamic_memory(buffer_size*sizeof(struct operation));
    buffer->posicaoBuffer = calloc(buffer_size, sizeof(int));
}

void inicializar_circularbuffer(struct circular_buffer *buffer, int buffer_size){
    buffer->buffer = malloc(buffer_size*sizeof(struct operation));
    buffer->posicoesEscritas = calloc(buffer_size,sizeof(int));
    buffer->posicaoEscrever = 0;
    buffer->posicaoLer = 0;

}

