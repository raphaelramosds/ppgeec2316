// gera um labirinto com a MST da grade de celulas, usando Kruskal e Prim (a partir da celula (0, 0))
// sobre o mesmo grafo, e compara os tempos de execucao
// uso: ./maze_main (linhas, colunas e semente sao lidas da entrada padrao)
#include "labirinto.h"
#include "mst.h"
#include "tempo.h"
#include <stdio.h>
#include <stdlib.h>

// fecha todas as paredes do labirinto
static void fechar_paredes(labirinto *L) {
    for (int k = 0; k < L->n_paredes; k++) {
        L->paredes[k].aberta = 0;
    }
}

int main(void) {
    int linhas, colunas;
    unsigned semente;
    int lidos = 0;
    printf("Linhas: ");
    lidos += scanf("%d", &linhas);
    printf("Colunas: ");
    lidos += scanf("%d", &colunas);
    printf("Semente: ");
    lidos += scanf("%u", &semente);
    if (lidos != 3 || linhas < 1 || colunas < 1) {
        fprintf(stderr, "Entrada invalida: linhas e colunas devem ser inteiros >= 1.\n");
        return 1;
    }
    printf("\n");
    srand(semente);

    // grade com todas as paredes fechadas e pesos aleatorios (compartilhada pelos dois algoritmos)
    labirinto *L = labirinto_criar(linhas, colunas);
    grafo *G = labirinto_grafo(L);

    aresta_mst *A = (aresta_mst*) malloc(G->n_vertices * sizeof(aresta_mst));
    // pi[v]: celula pela qual v entrou na arvore (-1 na raiz)
    int *pi = (int*) malloc(G->n_vertices * sizeof(int));
    if (A == NULL || pi == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }

    printf("Labirinto %dx%d (semente %u)\n", linhas, colunas, semente);

    // Kruskal
    int tam;
    double inicio = tempo_atual();
    double peso_kruskal = mst_kruskal(G, A, &tam);
    double tempo_kruskal = tempo_atual() - inicio;

    // cada aresta da MST vira uma passagem aberta
    for (int i = 0; i < tam; i++) {
        labirinto_abrir(L, A[i].u, A[i].v);
    }

    printf("\n[Kruskal]\n");
    printf("passagens abertas: %d de %d paredes, peso da MST: %g\n", tam, L->n_paredes, peso_kruskal);
    printf("tempo de execucao (mst_kruskal): %.3f ms\n", tempo_kruskal * 1e3);

    // Prim
    fechar_paredes(L);

    inicio = tempo_atual();
    double peso_prim = mst_prim(G, 0, pi);
    double tempo_prim = tempo_atual() - inicio;

    // cada aresta {pi[v], v} da MST vira uma passagem aberta
    int abertas = 0;
    for (int v = 0; v < G->n_vertices; v++) {
        if (pi[v] != -1) {
            labirinto_abrir(L, pi[v], v);
            abertas++;
        }
    }

    printf("\n[Prim]\n");
    printf("passagens abertas: %d de %d paredes, peso da MST: %g\n", abertas, L->n_paredes, peso_prim);
    printf("tempo de execucao (mst_prim): %.3f ms\n", tempo_prim * 1e3);

    free(A);
    free(pi);
    grafo_liberar(G);
    labirinto_liberar(L);
    return 0;
}
