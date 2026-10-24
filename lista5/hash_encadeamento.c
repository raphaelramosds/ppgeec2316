#include "hash_encadeamento.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

hash_encadeamento *encad_criar(int l) {
    hash_encadeamento *H = (hash_encadeamento*) malloc(sizeof(hash_encadeamento));
    H->l = l;
    H->m = 1 << l;
    H->n = 0;

    // todas as posicoes comecam com a lista vazia (NIL)
    H->T = (lista_encadeada*) calloc(H->m, sizeof(lista_encadeada));
    if (H->T == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    return H;
}

void encad_liberar(hash_encadeamento *H) {
    for (int j = 0; j < H->m; j++) {
        no_lista *x = H->T[j].head;
        while (x != NULL) {
            no_lista *prox = x->next;
            free(x);
            x = prox;
        }
    }
    free(H->T);
    free(H);
}

static inline int h(const hash_encadeamento *H, int k) {
    return multiply_shift((uint32_t) k, HASH_A, H->l);
}

void encad_inserir(hash_encadeamento *H, int k) {
    // LIST-PREPEND(T[h(k)], x)
    lista_inserir(&H->T[h(H, k)], k);
    H->n++;
}

no_lista *encad_buscar(const hash_encadeamento *H, int k) {
    // LIST-SEARCH(T[h(k)], k): percorre apenas a lista da posicao h(k), de tamanho medio alpha = n/m
    return lista_buscar(&H->T[h(H, k)], k);
}

void encad_remover(hash_encadeamento *H, no_lista *x) {
    // LIST-DELETE(T[h(x.key)], x)
    lista_remover(&H->T[h(H, x->chave)], x);
    free(x);
    H->n--;
}
