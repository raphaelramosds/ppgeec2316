#ifndef HEAP_H
#define HEAP_H

// Fila de prioridade minima sobre os vertices 0..n-1 com heap binario (CLRS secoes 6.5 e 21.2)
// A chave de cada vertice fica num vetor externo (v.key em Prim, v.d em Dijkstra), de modo que
// o algoritmo atualiza a chave e depois chama heap_diminuir_chave para restaurar a propriedade de heap
// pos[v] guarda o indice de v no heap, o que permite DECREASE-KEY em O(lg n) e o teste "v in Q" em O(1)
typedef struct {
    int *A;              // A[i]: vertice na posicao i do heap
    int *pos;            // pos[v]: posicao de v em A, ou -1 se v nao estiver no heap
    int tamanho;         // numero de elementos no heap
    const double *chave; // chave[v]: prioridade de v
} heap_min;

heap_min *heap_criar(int n, const double *chave);
void heap_liberar(heap_min *Q);

// INSERT: O(lg n)
void heap_inserir(heap_min *Q, int v);

// EXTRACT-MIN: remove e retorna o vertice de menor chave em O(lg n)
int heap_extrair_min(heap_min *Q);

// DECREASE-KEY: chamado apos chave[v] diminuir, O(lg n)
void heap_diminuir_chave(heap_min *Q, int v);

static inline int heap_vazio(const heap_min *Q) { return Q->tamanho == 0; }
static inline int heap_contem(const heap_min *Q, int v) { return Q->pos[v] != -1; }

#endif
