#ifndef DFS_H
#define DFS_H

#include "grafo.h"

#define BRANCO 0 // ainda nao descoberto
#define CINZA 1  // descoberto, mas a lista de adjacencia ainda nao foi toda examinada
#define PRETO 2  // finalizado

typedef struct {
    int *cor;
    int *d;     // d[u]: tempo de descoberta de u
    int *f;     // f[u]: tempo de finalizacao de u
    int *pi;    // pi[u]: predecessor de u na floresta de profundidade (-1 = NIL)
    int tempo;
} dfs_resultado;

dfs_resultado *dfs(const grafo *G);
void dfs_liberar(dfs_resultado *R);

#endif
