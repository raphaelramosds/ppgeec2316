#ifndef DFS_H
#define DFS_H

#include "grafo.h"

#define BRANCO 0 // ainda nao descoberto
#define CINZA 1  // descoberto, mas a lista de adjacencia ainda nao foi toda examinada
#define PRETO 2  // finalizado

// Resultado da busca em profundidade (CLRS secao 20.3)
typedef struct {
    int *cor;
    int *d;     // d[u]: tempo de descoberta de u
    int *f;     // f[u]: tempo de finalizacao de u
    int *pi;    // pi[u]: predecessor de u na floresta de profundidade (-1 = NIL)
    int tempo;
    int ciclo;  // 1 se alguma aresta de retorno (back edge) foi encontrada
} dfs_resultado;

// DFS: Theta(V + E). Os vertices sao percorridos em ordem crescente e as listas de adjacencia
// na ordem em que as arestas foram inseridas
dfs_resultado *dfs(const grafo *G);
void dfs_liberar(dfs_resultado *R);

// TOPOLOGICAL-SORT (CLRS secao 20.4): Theta(V + E)
// Preenche ordem[0..n-1] com os vertices ordenados topologicamente (f decrescente)
// Retorna 0 em caso de sucesso ou -1 se G tiver ciclo (Lema 20.11: G eh aciclico <=> DFS nao gera aresta de retorno)
int ordenacao_topologica(const grafo *G, int *ordem);

#endif
