#include <stdlib.h>
#include "cache_random.h"

// Criar bloco com identificador; todo bloco recem inserido entra marcado
static Block *create_marked_block(int id) {
    Block *b = (Block *)malloc(sizeof(Block));
    if (!b) exit(EXIT_FAILURE);
    b->id = id;
    b->ts = 1;
    b->mark = 1;
    b->next = NULL;
    return b;
}

// Marcacao aleatoria (randomized marking):
//  - se todos os blocos do cache estao marcados, inicia uma nova fase desmarcando todos
//  - sorteia uniformemente um bloco NAO marcado e o remove
//  - insere o novo bloco b (marcado) no cache
void random_evict(Cache *c, Block *b) {
    if (c == NULL || b == NULL) return;

    // conta os blocos nao marcados
    int unmarked = 0;
    for (Block *curr = c->head; curr != NULL; curr = curr->next) {
        if (curr->mark == 0) unmarked++;
    }

    // todos marcados: fim da fase, desmarca todos
    if (unmarked == 0) {
        for (Block *curr = c->head; curr != NULL; curr = curr->next) {
            curr->mark = 0;
        }
        unmarked = c->size;
    }

    if (unmarked > 0) {
        // sorteia o r-esimo bloco nao marcado como vitima
        int r = rand() % unmarked;

        Block *prev = NULL;
        Block *curr = c->head;
        while (curr != NULL) {
            if (curr->mark == 0) {
                if (r == 0) break;
                r--;
            }
            prev = curr;
            curr = curr->next;
        }

        // remocao da vitima (curr)
        if (prev == NULL) {
            c->head = curr->next;
        } else {
            prev->next = curr->next;
        }
        free(curr);
        c->size -= 1;
    }

    // insercao do novo bloco marcado na cabeca da lista
    b->mark = 1;
    b->next = c->head;
    c->head = b;
    c->size += 1;
}

// Equivalente ao lru_get_block, mas usando a politica de marcacao aleatoria
int random_get_block(Cache *c, int block_id) {
    for (Block *curr = c->head; curr != NULL; curr = curr->next) {
        if (curr->id == block_id) {
            // cache hit: marca o bloco
            curr->ts += 1;
            curr->mark = 1;
            return 1;
        }
    }

    // cache miss
    c->misses += 1;
    if (c->capacity <= 0) {
        return 0;
    }

    Block *novo = create_marked_block(block_id);
    if (c->size >= c->capacity) {
        // remove um bloco nao marcado aleatorio e insere o novo
        random_evict(c, novo);
    } else {
        novo->next = c->head;
        c->head = novo;
        c->size += 1;
    }
    return 0;
}
