#ifndef CACHE_H
#define CACHE_H

#include <stddef.h>
#include <stdlib.h>

typedef struct block {
    int id;
    int ts;
    struct block *next;
} Block;

typedef struct cache {
    Block *head;
    int size;       // numero de blocos inseridos
    int capacity;   // tamanho maximo (k)
    int misses;
} Cache;

// Aloca e inicializa um cache vazio com capacidade k
static inline Cache *init_cache(int capacity) {
    Cache *c = (Cache *)malloc(sizeof(Cache));
    // Retorna NULL em caso de falha na alocacao
    if (!c) return NULL;
    c->head = NULL;
    c->size = 0;
    c->capacity = capacity;
    c->misses = 0;
    return c;
}

// Libera todos os blocos e o proprio cache
static inline void free_cache(Cache *c) {
    if (!c) return;
    Block *curr = c->head;
    while (curr != NULL) {
        Block *next = curr->next;
        free(curr);
        curr = next;
    }
    free(c);
}

// Aplica alguma politica de remocao de blocos no cache
void evict(Cache *c);

// Busca o bloco block_id no cache
// Funcionamento: 
//  - Se encontrado (hit): incrementa ts desse bloco e retorna 1
//  - Se nao encontrado (miss): incrementa misses e retorna 0. E se houver espaço livre, insere o bloco com ts = 1, caso contrario, chama evict(c) antes de inserir
int get_block(Cache *c, int block_id);

#endif