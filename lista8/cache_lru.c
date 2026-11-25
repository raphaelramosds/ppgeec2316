#include <stdlib.h>
#include "cache.h"

// Criar bloco com identificador e timestamp
static Block *create_block(int id, int ts) {
    Block *b = (Block *)malloc(sizeof(Block));
    if (!b) exit(EXIT_FAILURE);
    b->id = id;
    b->ts = ts;
    b->next = NULL;
    return b;
}

void evict(Cache *c) {
    if (c == NULL || c->head == NULL) return;

    Block *prev = NULL;
    Block *curr = c->head;

    // mantem a referencia do node com menor ts (min_node) e do seu anterior (min_prev)
    Block *min_prev = NULL;
    Block *min_node = c->head;

    while (curr != NULL) {
        // procure o bloco menos utilizado (o que possuir menor timestamp)
        if (curr->ts < min_node->ts) {
            min_node = curr;
            min_prev = prev;
        }
        // proximo bloco
        prev = curr;
        curr = curr->next;
    }

    // remocao do bloco menos utilizado (min_node)
    if (min_prev == NULL) {
        // o min_node ja esta na cabeca da lista encadeada, entao simplesmente atualize a cabeca
        c->head = min_node->next;
    } else {
        // o bloco anterior (min_prev) passa a apontar para o proximo do bloco menos utilizado (min_node)
        min_prev->next = min_node->next;
    }

    // desaloque o min_node da heap
    free(min_node);
    c->size -= 1;
}

int get_block(Cache *c, int block_id) {
    Block *curr = c->head;
    while (curr != NULL) {
        if (curr->id == block_id) {
            // cache hit
            curr->ts += 1;
            return 1;
        }
        curr = curr->next;
    }

    // cache miss
    c->misses += 1;
    if (c->capacity <= 0) {
        return 0;
    }

    if (c->size >= c->capacity) {
        evict(c);
    }

    Block *novo = create_block(block_id, 1);
    novo->next = c->head;
    c->head = novo;
    c->size += 1;
    return 0;
}
