#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

grafo *grafo_criar(int n, tipo_grafo dirigido) {
    grafo *G = (grafo*) malloc(sizeof(grafo));
    G->n_vertices = n;
    G->n_arestas = 0;
    G->dirigido = dirigido;
    G->adj = (aresta**) calloc(n, sizeof(aresta*));
    G->fim = (aresta**) calloc(n, sizeof(aresta*));
    if (G->adj == NULL || G->fim == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    return G;
}

void grafo_liberar(grafo *G) {
    for (int u = 0; u < G->n_vertices; u++) {
        aresta *a = G->adj[u];
        while (a != NULL) {
            aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(G->adj);
    free(G->fim);
    free(G);
}

// insere no final da lista de adjacencia
static void inserir(grafo *G, int u, int v, double w) {
    aresta *a = (aresta*) malloc(sizeof(aresta));
    a->v = v;
    a->w = w;
    a->prox = NULL;
    if (G->fim[u] == NULL) {
        // lista vazia, posicione na cabeca da lista
        G->adj[u] = a;
    } else {
        // faca a "antiga" ultima aresta apontar para esta aresta que vamos inserir
        G->fim[u]->prox = a;
    }

    // esta aresta passa a ser a ultima
    G->fim[u] = a;
}

void grafo_adicionar_aresta(grafo *G, int u, int v, double w) {
    inserir(G, u, v, w);
    if (G->dirigido == NAO_DIRIGIDO && u != v) {
        // se nao for dirigido, coloque na lista de adjacencia de v tambem 
        inserir(G, v, u, w);
    }
    G->n_arestas++;
}
