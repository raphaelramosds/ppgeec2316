#ifndef SELECTION_H
#define SELECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Algoritmos de selecao disponiveis em medir_tempo
#define RANDOMIZED 1
#define MEDIANA 2

// Encontre o i-esimo menor elemento em A[p:r] utilizando selecao aleatoria do pivo
int randomized_select(int A[], int p, int r, int i);

// Encontre o i-esimo menor elemento em A[p:r] utilizando a selecao do melhor pivo pela mediana das medianas dos grupos
int median_select(int A[], int p, int r, int i);


// Executa o algoritmo "repeticoes" vezes sobre copias de A e retorna o tempo medio de uma chamada
// NOTE: os algoritmos modificam o vetor, entao cada repeticao recebe uma copia nova
static inline double medir_tempo(const int A[], int n, int i, int algoritmo, int repeticoes) {
    int *copia = (int*) malloc(n * sizeof(int));
    double total = 0.0;

    for (int rep = 0; rep < repeticoes; rep++) {
        memcpy(copia, A, n * sizeof(int));

        clock_t inicio = clock();

        if (algoritmo == RANDOMIZED) {
            randomized_select(copia, 0, n - 1, i);
        } else {
            median_select(copia, 0, n - 1, i);
        }

        clock_t fim = clock();
        total += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    }

    free(copia);
    return total / repeticoes;
}

#endif