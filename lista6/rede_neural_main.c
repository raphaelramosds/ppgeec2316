// monta o grafo de uma rede neural totalmente conectada (I entradas, W camadas ocultas de n neuronios
// e O saidas), com pesos aleatorios, e compara os tempos de execucao de Kruskal e Prim sobre ele
// uso: ./rede_neural_main (I, W, n, O e semente sao lidos da entrada padrao)
#include "mst.h"
#include "rede_neural.h"
#include "tempo.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int I, W, n, O;
    unsigned semente;
    int lidos = 0;
    printf("Entradas (I): ");
    lidos += scanf("%d", &I);
    printf("Camadas ocultas (W): ");
    lidos += scanf("%d", &W);
    printf("Neuronios por camada oculta (n): ");
    lidos += scanf("%d", &n);
    printf("Saidas (O): ");
    lidos += scanf("%d", &O);
    printf("Semente: ");
    lidos += scanf("%u", &semente);
    if (lidos != 5 || I < 1 || W < 0 || n < 1 || O < 1) {
        fprintf(stderr, "Entrada invalida: I, n e O devem ser >= 1 e W >= 0.\n");
        return 1;
    }
    printf("\n");
    srand(semente);

    // mesmo grafo para os dois algoritmos
    grafo *G = rede_neural_grafo(I, W, n, O);

    aresta_mst *A = (aresta_mst*) malloc(G->n_vertices * sizeof(aresta_mst));
    int *pi = (int*) malloc(G->n_vertices * sizeof(int));
    if (A == NULL || pi == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }

    printf("Rede neural: %d entradas, %d camadas ocultas de %d neuronios, %d saidas (semente %u)\n", I, W, n, O, semente);
    printf("|V| = %d, |E| = %d\n", G->n_vertices, G->n_arestas);

    // Kruskal
    int tam;
    double inicio = tempo_atual();
    double peso_kruskal = mst_kruskal(G, A, &tam);
    double tempo_kruskal = tempo_atual() - inicio;

    printf("\n[Kruskal]\n");
    printf("arestas na MST: %d, peso da MST: %g\n", tam, peso_kruskal);
    printf("tempo de execucao (mst_kruskal): %.3f ms\n", tempo_kruskal * 1e3);

    // Prim, a partir do primeiro vertice de entrada
    inicio = tempo_atual();
    double peso_prim = mst_prim(G, 0, pi);
    double tempo_prim = tempo_atual() - inicio;

    int arestas_prim = 0;
    for (int v = 0; v < G->n_vertices; v++) {
        if (pi[v] != -1) {
            arestas_prim++;
        }
    }

    printf("\n[Prim]\n");
    printf("arestas na MST: %d, peso da MST: %g\n", arestas_prim, peso_prim);
    printf("tempo de execucao (mst_prim): %.3f ms\n", tempo_prim * 1e3);

    free(A);
    free(pi);
    grafo_liberar(G);
    return 0;
}
