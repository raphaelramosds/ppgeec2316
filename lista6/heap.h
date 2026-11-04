#ifndef HEAP_H
#define HEAP_H

typedef struct {
    int *A;              // A[i]: valor do node na posicao i do heap
    int *heap_index;     // heap_index[v]: posicao de v em A, ou -1 se v nao estiver no heap
    int tamanho;         // numero de elementos no heap
    const double *prioridades; // prioridades[v]: prioridade de v
} heap_min;

heap_min *heap_criar(int n, const double *prioridades);
void heap_liberar(heap_min *Q);

void heap_inserir(heap_min *Q, int v);

// remove e retorna o vertice de menor chave
int heap_extrair_min(heap_min *Q);

// sobe o node na lista de prioridade baseando-se na sua chave atualizada
// NOTE: esta funcao assume que a prioridade do node v no array "prioridades"
// foi atualizada externamente com um valor menor do que o que ela possuia
void heap_diminuir_chave(heap_min *Q, int v);

static inline int heap_vazio(const heap_min *Q) { return Q->tamanho == 0; }
static inline int heap_contem(const heap_min *Q, int v) { return Q->heap_index[v] != -1; }

#endif
