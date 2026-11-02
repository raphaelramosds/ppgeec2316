#ifndef MST_H
#define MST_H

#include "grafo.h"

typedef struct {
    // aresta { u, v} de peso w pertencente a arvore geradora minima
    int u, v;
    double w;
} aresta_mst;

double mst_kruskal(const grafo *G, aresta_mst *A, int *tam);

double mst_prim(const grafo *G, int r, int *pi);

#endif
