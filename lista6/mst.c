#include "mst.h"
#include "conjuntos.h"
#include "heap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int comparar_peso(const void *a, const void *b) {
    double wa = ((const aresta_mst*) a)->w, wb = ((const aresta_mst*) b)->w;
    return (wa > wb) - (wa < wb);
}

double mst_kruskal(const grafo *G, aresta_mst *A, int *tam) {
    node *S = (node*) malloc(G->n_vertices * sizeof(node)); // S[v] eh o elemento do conjunto que representa o vertice v
    aresta_mst *E = (aresta_mst*) malloc((G->n_arestas > 0 ? G->n_arestas : 1) * sizeof(aresta_mst));
    if (S == NULL || E == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }

    // cada vertice vira um conjunto disjunto
    for (int v = 0; v < G->n_vertices; v++) {
        make_set(&S[v]);
    }

    // cria a lista de arestas sem repeticao
    // NOTE: (u,v) aparece em adj[u] e (v, u) em adj[v], entao insere-se o menor (u < v)
    int m = 0;
    for (int u = 0; u < G->n_vertices; u++) {
        for (aresta *a = G->adj[u]; a != NULL; a = a->prox) {
            if (u < a->v) {
                E[m].u = u;
                E[m].v = a->v;
                E[m].w = a->w;
                m++;
            }
        }
    }

    // ordena as arestas em ordem monotonicamente crescente de peso
    qsort(E, m, sizeof(aresta_mst), comparar_peso);

    double total = 0.0;
    *tam = 0;
    for (int i = 0; i < m; i++) {
        // NOTE: find_set recebe um ponteiro para o node do conjunto disjunto, e nao o indice do vertice
        // por isso, usamos o indice E[i].u para acessar o node S[E[i].u] e passamos o seu endereco (&)
        if (find_set(&S[E[i].u]) != find_set(&S[E[i].v])) {
            A[(*tam)++] = E[i];
            total += E[i].w;
            union_set(&S[E[i].u], &S[E[i].v]);
        }
    }

    free(S);
    free(E);
    return total;
}


double mst_prim(const grafo *G, int r, int *pi) {
    // prioridades de cada vertice
    double *keys = (double*) malloc(G->n_vertices * sizeof(double));

    if (keys == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    for (int u = 0; u < G->n_vertices; u++) {
        keys[u] = INFINITY;
        pi[u] = -1;
    }

    // prioridade da raiz eh zero (menor possivel)
    keys[r] = 0.0;

    heap_min *Q = heap_criar(G->n_vertices, keys);
    for (int u = 0; u < G->n_vertices; u++) {
        heap_inserir(Q, u);
    }

    double total = 0.0;
    while (!heap_vazio(Q)) {
        int u = heap_extrair_min(Q); // adiciona u a arvore
        if (keys[u] != INFINITY) {
            total += keys[u];
        }
        // atualiza as chaves dos vizinhos de u fora da arvore/
        for (aresta *a = G->adj[u]; a != NULL; a = a->prox) { 
            int v = a->v;
            if (heap_contem(Q, v) && a->w < keys[v]) {
                pi[v] = u;
                keys[v] = a->w;
                heap_diminuir_chave(Q, v);
            }
        }
    }

    heap_liberar(Q);
    free(keys);
    return total;
}
