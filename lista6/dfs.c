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

// DFS-VISIT(G, u). Se ordem != NULL, cada vertice finalizado eh inserido na frente da lista
// (aqui, preenchendo o vetor ordem de tras para frente a partir de *k)
static void dfs_visit(const grafo *G, dfs_resultado *R, int u, int *ordem, int *k) {
    R->tempo++;                 // u acabou de ser descoberto
    R->d[u] = R->tempo;
    R->cor[u] = CINZA;
    for (aresta *a = G->adj[u]; a != NULL; a = a->prox) { // explora cada aresta (u, v)
        int v = a->v;
        if (R->cor[v] == BRANCO) {
            R->pi[v] = u;
            dfs_visit(G, R, v, ordem, k);
        } else if (R->cor[v] == CINZA) {
            R->ciclo = 1;       // (u, v) eh aresta de retorno
        }
    }
    R->tempo++;
    R->f[u] = R->tempo;
    R->cor[u] = PRETO;          // u finalizado
    if (ordem != NULL) {
        ordem[--(*k)] = u;
    }
}

static dfs_resultado *dfs_ordem(const grafo *G, int *ordem) {
    dfs_resultado *R = dfs_alocar(G->n_vertices);
    for (int u = 0; u < G->n_vertices; u++) {
        R->cor[u] = BRANCO;
        R->pi[u] = -1;
    }
    R->tempo = 0;
    R->ciclo = 0;
    int k = G->n_vertices;
    for (int u = 0; u < G->n_vertices; u++) {
        if (R->cor[u] == BRANCO) {
            dfs_visit(G, R, u, ordem, &k);
        }
    }
    return R;
}

dfs_resultado *dfs(const grafo *G) {
    return dfs_ordem(G, NULL);
}

int ordenacao_topologica(const grafo *G, int *ordem) {
    dfs_resultado *R = dfs_ordem(G, ordem);
    int ciclo = R->ciclo;
    dfs_liberar(R);
    return ciclo ? -1 : 0;
}
