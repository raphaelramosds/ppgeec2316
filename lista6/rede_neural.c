#include "rede_neural.h"
#include <stdio.h>
#include <stdlib.h>

grafo *rede_neural_grafo(int I, int W, int n, int O) {
    int n_camadas = W + 2; // W + (1 camada de entrada) + (1 camada de saida)
    int *tamanho = (int*) malloc(n_camadas * sizeof(int));
    int *inicio = (int*) malloc(n_camadas * sizeof(int)); // posicao do inicio de cada camada
    if (tamanho == NULL || inicio == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }

    // tamanho da camada de entrada, de cada camada oculta e da camada de saida
    tamanho[0] = I;
    for (int c = 1; c <= W; c++) {
        // cada camada oculta tem n vertices
        tamanho[c] = n;
    }
    tamanho[W + 1] = O;

    // preencher onde cada camada comeca
    int n_vertices = 0;
    for (int c = 0; c < n_camadas; c++) {
        inicio[c] = n_vertices;
        n_vertices += tamanho[c];
    }

    // MST exige grafo nao dirigido
    grafo *G = grafo_criar(n_vertices, NAO_DIRIGIDO);

    // cada vertice da camada c se conecta a todos os da camada c+1
    for (int c = 0; c + 1 < n_camadas; c++) {
        // para cada i-esimo neuronio da camada atual (c)
        for (int i = 0; i < tamanho[c]; i++) {
            // ligue-o com os neuronios da proxima camada (c+1)
            // NOTE: inicio[c+1] eh o deslocamento necessario para acessar o primeiro neuronio da proxima camada
            for (int j = 0; j < tamanho[c + 1]; j++) {
                // peso aleatorio
                double w = (double) rand() / ((double) RAND_MAX + 1.0);
                grafo_adicionar_aresta(G, inicio[c] + i, inicio[c + 1] + j, w);
            }
        }
    }

    free(tamanho);
    free(inicio);
    return G;
}
