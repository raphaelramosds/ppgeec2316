#ifndef CACHE_H
#define CACHE_H

#include <stddef.h>
#include <stdlib.h>

typedef struct block {
    int id;
    int ts;
    int mark;       // 0 ou 1
    struct block *next;
} Block;

typedef struct cache {
    Block *head;
    int size;       // numero de blocos inseridos
    int capacity;   // tamanho maximo (k)
    int misses;
    int clock;      // relogio logico: incrementado a cada acesso ao cache
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
    c->clock = 0;
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

#endif
