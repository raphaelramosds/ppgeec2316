#ifndef GRAFO_H
#define GRAFO_H

typedef struct aresta {
    int v;                  // vertice de destino
    double w;               // peso da aresta (u, v)
    struct aresta *prox;    // ponteiro para o proximo vertice da lista de adjacencia
} aresta;

typedef struct {
    int n_vertices;     // numero de vertices |V|
    int n_arestas;      // numero de arestas |E|
    int dirigido;       // 1: dirigido; 0: nao dirigido
    aresta **adj;       // adj[u]: lista de adjacencia de u
    aresta **fim;       // fim[u]: ultima aresta de adj[u], para inserir no final em O(1)
} grafo;

grafo *grafo_criar(int n, int dirigido);
void grafo_liberar(grafo *G);
void grafo_adicionar_aresta(grafo *G, int u, int v, double w);

#endif
