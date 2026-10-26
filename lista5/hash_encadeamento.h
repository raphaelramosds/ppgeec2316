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

// Busca k na lista T[h(k)] e remove seu no com CHAINED-HASH-DELETE (O(1)), liberando-o.
// Theta(1 + alpha) no caso medio, dominado pela busca. Retorna a posicao h(k) ou -1 se k nao
// estiver na tabela
int encad_remover(hash_encadeamento *H, int k);

#endif
