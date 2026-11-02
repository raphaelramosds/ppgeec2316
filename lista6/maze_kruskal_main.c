// gera um labirinto com a MST de Kruskal sobre a grade de celulas
// uso: ./maze_kruskal_main linhas [colunas] [-s semente] [-d]
#include "labirinto.h"
#include "mst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int linhas, colunas;
    unsigned semente;
    int desenhar;
    if (labirinto_ler_argumentos(argc, argv, &linhas, &colunas, &semente, &desenhar) != 0) {
        fprintf(stderr, "uso: %s linhas [colunas] [-s semente] [-d]\n", argv[0]);
        return 1;
    }
    srand(semente);

    // grade com todas as paredes fechadas e pesos aleatorios
    labirinto *L = labirinto_criar(linhas, colunas);
    grafo *G = labirinto_grafo(L);

    aresta_mst *A = (aresta_mst*) malloc(G->n_vertices * sizeof(aresta_mst));
    if (A == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }
    int tam;

    // peso total da arvore
    double peso = mst_kruskal(G, A, &tam);

    // cada aresta da MST vira uma passagem aberta
    for (int i = 0; i < tam; i++) {
        labirinto_abrir(L, A[i].u, A[i].v);
    }

    printf("Labirinto %dx%d (Kruskal, semente %u)\n", linhas, colunas, semente);
    printf("passagens abertas: %d de %d paredes, peso da MST: %g\n", tam, L->n_paredes, peso);

    free(A);
    grafo_liberar(G);
    labirinto_liberar(L);
    return 0;
}
