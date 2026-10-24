#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

// Lista duplamente encadeada nao ordenada (CLRS secao 10.2)
typedef struct no_lista {
    int chave;
    struct no_lista *prev;
    struct no_lista *next;
} no_lista;

typedef struct {
    no_lista *head;
} lista_encadeada;

lista_encadeada *lista_criar(void);
void lista_liberar(lista_encadeada *L);

// LIST-PREPEND: insere x na cabeca da lista em O(1)
void lista_inserir(lista_encadeada *L, int x);

// LIST-SEARCH: busca linear, O(n) no pior caso. Retorna o no ou NULL
no_lista *lista_buscar(const lista_encadeada *L, int k);

#endif
