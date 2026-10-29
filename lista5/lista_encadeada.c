#include "lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>

lista_encadeada *lista_criar(void) {
    lista_encadeada *L = (lista_encadeada*) malloc(sizeof(lista_encadeada));
    L->head = NULL;
    return L;
}

void lista_liberar(lista_encadeada *L) {
    no_lista *x = L->head;
    while (x != NULL) {
        no_lista *prox = x->next;
        free(x);
        x = prox;
    }
    free(L);
}

void lista_inserir(lista_encadeada *L, int k) {
    no_lista *x = (no_lista*) malloc(sizeof(no_lista));
    if (x == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    x->chave = k;

    x->next = L->head;
    x->prev = NULL;
    if (L->head != NULL) {
        L->head->prev = x;
    }
    L->head = x;
}

no_lista *lista_buscar(const lista_encadeada *L, int k) {
    // como a lista nao eh ordenada, precisamos percorrer ate achar k ou chegar ao fim
    no_lista *x = L->head;
    while (x != NULL && x->chave != k) {
        x = x->next;
    }
    return x;
}

void lista_remover(lista_encadeada *L, no_lista *x) {
    // como a lista eh duplamente encadeada, basta religar o anterior e o proximo de x
    if (x->prev != NULL) {
        x->prev->next = x->next;
    } else {
        L->head = x->next;
    }
    if (x->next != NULL) {
        x->next->prev = x->prev;
    }
}
