#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)

heap_min *heap_criar(int n, const double *prioridades) {
    heap_min *Q = (heap_min*) malloc(sizeof(heap_min));
    Q->A = (int*) malloc(n * sizeof(int));
    Q->heap_index = (int*) malloc(n * sizeof(int));
    if (Q->A == NULL || Q->heap_index == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    for (int v = 0; v < n; v++) {
        Q->heap_index[v] = -1;
    }
    Q->tamanho = 0;
    Q->prioridades = prioridades;
    return Q;
}

void heap_liberar(heap_min *Q) {
    free(Q->A);
    free(Q->heap_index);
    free(Q);
}

static void trocar(heap_min *Q, int i, int j) {
    // troca nos no array de heap
    int t = Q->A[i];
    Q->A[i] = Q->A[j];
    Q->A[j] = t;

    // troca posicoes
    Q->heap_index[Q->A[i]] = i;
    Q->heap_index[Q->A[j]] = j;
}

// sobe o elemento da posicao i enquanto ele for menor que o pai
static void bubble_up(heap_min *Q, int i) {
    while (i > 0 && Q->prioridades[Q->A[PARENT(i)]] > Q->prioridades[Q->A[i]]) {
        trocar(Q, i, PARENT(i));
        i = PARENT(i);
    }
}

static void min_heapify(heap_min *Q, int i) {
    int l = LEFT(i), r = RIGHT(i), menor = i;
    if (l < Q->tamanho && Q->prioridades[Q->A[l]] < Q->prioridades[Q->A[menor]]) menor = l;
    if (r < Q->tamanho && Q->prioridades[Q->A[r]] < Q->prioridades[Q->A[menor]]) menor = r;
    if (menor != i) {
        trocar(Q, i, menor);
        min_heapify(Q, menor);
    }
}

void heap_inserir(heap_min *Q, int v) {
    Q->A[Q->tamanho] = v;
    Q->heap_index[v] = Q->tamanho;
    Q->tamanho++;
    bubble_up(Q, Q->tamanho - 1);
}

int heap_extrair_min(heap_min *Q) {
    int min = Q->A[0];
    Q->tamanho--;
    if (Q->tamanho > 0) {
        Q->A[0] = Q->A[Q->tamanho];
        Q->heap_index[Q->A[0]] = 0;
        min_heapify(Q, 0);
    }
    Q->heap_index[min] = -1;
    return min;
}

void heap_diminuir_chave(heap_min *Q, int v) {
    bubble_up(Q, Q->heap_index[v]);
}
