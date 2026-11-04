#ifndef MST_H
#define MST_H

#include "grafo.h"

typedef struct {
    // aresta (u, v) de peso w pertencente a arvore geradora minima
    int u, v;
    double w;
} aresta_mst;

// algoritmo de kruskal (G nao dirigido)
// saida: preenche A com as arestas da arvore geradora minima e tam com a quantidade delas
// retorno: peso total da arvore
double mst_kruskal(const grafo *G, aresta_mst *A, int *tam);

// algoritmo de prim (G nao dirigido), a partir da fonte r
// saida: preenche pi com o predecessor de cada vertice na arvore geradora minima (pi[r] = -1)
// retorno: peso total da arvore
double mst_prim(const grafo *G, int r, int *pi);

#endif