#ifndef HASH_ENCADEAMENTO_H
#define HASH_ENCADEAMENTO_H

#include "lista_encadeada.h"

// Tabela hash com resolucao de colisoes por encadeamento (CLRS secao 11.2)
// NOTE: cada posicao T[j] eh uma lista duplamente encadeada com as chaves de hash j
typedef struct {
    lista_encadeada *T;
    int m; // numero de posicoes, m = 2^l
    int l;
    int n; // numero de chaves armazenadas
} hash_encadeamento;

hash_encadeamento *encad_criar(int l);
void encad_liberar(hash_encadeamento *H);

// CHAINED-HASH-INSERT: O(1) no pior caso. Assume que k ainda nao esta na tabela
void encad_inserir(hash_encadeamento *H, int k);

// CHAINED-HASH-SEARCH: Theta(1 + alpha) no caso medio. Retorna o no ou NULL
no_lista *encad_buscar(const hash_encadeamento *H, int k);

// CHAINED-HASH-DELETE: O(1), recebe o no x (e nao a chave) e o libera
void encad_remover(hash_encadeamento *H, no_lista *x);

#endif
