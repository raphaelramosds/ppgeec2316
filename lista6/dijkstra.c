#include "dijkstra.h"
#include "heap.h"
#include <math.h>
#include <stdio.h>

static void initialize_single_source(const grafo *G, int s, double *d, int *pi) {
    for (int v = 0; v < G->n_vertices; v++) {
        d[v] = INFINITY;
        pi[v] = -1;
    }
    d[s] = 0.0;
}

static int relax(int u, int v, double w, double *d, int *pi) {
    if (d[v] > d[u] + w) {
        d[v] = d[u] + w;
        pi[v] = u;
        return 1;
    }
    return 0;
}

void dijkstra(const grafo *G, int s, double *d, int *pi) {
    initialize_single_source(G, s, d, pi);

    heap_min *Q = heap_criar(G->n_vertices, d);
    for (int u = 0; u < G->n_vertices; u++) {
        heap_inserir(Q, u);
    }

    while (!heap_vazio(Q)) {
        int u = heap_extrair_min(Q);
        for (aresta *a = G->adj[u]; a != NULL; a = a->prox) {
            if (relax(u, a->v, a->w, d, pi) && heap_contem(Q, a->v)) {
                heap_diminuir_chave(Q, a->v);
            }
        }
    }

    heap_liberar(Q);
}

void imprimir_caminho(const grafo *G, const int *pi, int s, int v) {
    if (v == s) {
        printf("%d", s);
    } else if (pi[v] == -1) {
        printf("nao existe caminho de %d a %d", s, v);
    } else {
        imprimir_caminho(G, pi, s, pi[v]);
        printf(" -> %d", v);
    }
}
