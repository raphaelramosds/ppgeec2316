// gera um labirinto com a MST de Prim sobre a grade de celulas, a partir da celula (0, 0)
// uso: ./maze_prim_main linhas [colunas] [-s semente] [-d]
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

    // pi[v]: celula pela qual v entrou na arvore (-1 na raiz)
    int *pi = (int*) malloc(G->n_vertices * sizeof(int));
    if (pi == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }

    // peso total da arvore
    double inicio = labirinto_tempo();
    double peso = mst_prim(G, 0, pi);
    double tempo = labirinto_tempo() - inicio;

    // cada aresta {pi[v], v} da MST vira uma passagem aberta
    int abertas = 0;
    for (int v = 0; v < G->n_vertices; v++) {
        if (pi[v] != -1) {
            labirinto_abrir(L, pi[v], v);
            abertas++;
        }
    }

    printf("Labirinto %dx%d (Prim, semente %u)\n", linhas, colunas, semente);
    printf("passagens abertas: %d de %d paredes, peso da MST: %g\n", abertas, L->n_paredes, peso);
    printf("tempo de execucao (mst_prim): %.3f ms\n", tempo * 1e3);

    free(pi);
    grafo_liberar(G);
    labirinto_liberar(L);
    return 0;
}
