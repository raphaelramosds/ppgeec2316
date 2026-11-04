#include "dfs.h"
#include <stdio.h>
#include <stdlib.h>

static dfs_resultado *dfs_alocar(int n) {
    dfs_resultado *R = (dfs_resultado*) malloc(sizeof(dfs_resultado));
    R->cor = (int*) malloc(n * sizeof(int));
    R->d = (int*) malloc(n * sizeof(int));
    R->f = (int*) malloc(n * sizeof(int));
    R->pi = (int*) malloc(n * sizeof(int));
    if (R->cor == NULL || R->d == NULL || R->f == NULL || R->pi == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    return R;
}

void dfs_liberar(dfs_resultado *R) {
    free(R->cor);
    free(R->d);
    free(R->f);
    free(R->pi);
    free(R);
}

static void dfs_visit(const grafo *G, dfs_resultado *R, int u) {
    // u acabou de ser descoberto
    R->tempo++;
    R->d[u] = R->tempo;
    R->cor[u] = CINZA;

    // explora cada aresta (u, v)
    for (aresta *a = G->adj[u]; a != NULL; a = a->prox) {
        int v = a->v;
        if (R->cor[v] == BRANCO) {
            R->pi[v] = u;
            dfs_visit(G, R, v);
        }
    }

    // u finalizado
    R->tempo++;
    R->f[u] = R->tempo;
    R->cor[u] = PRETO;
}

dfs_resultado *dfs(const grafo *G) {
    dfs_resultado *R = dfs_alocar(G->n_vertices);
    for (int u = 0; u < G->n_vertices; u++) {
        R->cor[u] = BRANCO;
        R->pi[u] = -1;
    }
    R->tempo = 0;
    for (int u = 0; u < G->n_vertices; u++) {
        if (R->cor[u] == BRANCO) {
            dfs_visit(G, R, u);
        }
    }
    return R;
}
