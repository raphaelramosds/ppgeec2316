#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

grafo *grafo_criar(int n, tipo_grafo dirigido) {
    grafo *G = (grafo*) malloc(sizeof(grafo));
    G->n_vertices = n;
    G->n_arestas = 0;
    G->dirigido = dirigido;
    G->adj = (aresta**) calloc(n, sizeof(aresta*));
    if (G->adj == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    return G;
}

void grafo_liberar(grafo *G) {
    for (int u = 0; u < G->n_vertices; u++) {
        aresta *a = G->adj[u];
        while (a != NULL) {
            // desaloque todas as arestas da lista de adjacencia de u
            aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(G->adj);
    free(G);
}

// insere no inicio da lista de adjacencia, em O(1)
static void inserir(grafo *G, int u, int v, double w) {
    aresta *a = (aresta*) malloc(sizeof(aresta));
    a->v = v;
    a->w = w;
    a->prox = G->adj[u];    // nova aresta aponta para a antiga cabeca da lista
    G->adj[u] = a;          // e passa a ser a nova cabeca
}

void grafo_adicionar_aresta(grafo *G, int u, int v, double w) {
    inserir(G, u, v, w);
    if (G->dirigido == NAO_DIRIGIDO && u != v) {
        // se nao for dirigido, coloque na lista de adjacencia de v tambem 
        inserir(G, v, u, w);
    }
    G->n_arestas++;
}
