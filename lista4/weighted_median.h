#ifndef WEIGHTEDMEDIAN_H
#define WEIGHTEDMEDIAN_H

#include <stdio.h>
#include <stdlib.h>

// definicao da estrutura para representar o par (valor, peso)
typedef struct {
    double valor;
    double peso;
} elemento;

int compare(const void *a, const void *b) {
    // funcao de ordenacao customizada para ordenar um conjunto de estruturas
    const elemento *el1 = (const elemento *)a;
    const elemento *el2 = (const elemento *)b;

    // ordena em ordem crescente pelo valor
    if (el1->valor < el2->valor) return -1;
    if (el1->valor > el2->valor) return 1;
    return 0; // iguais
}

static inline double weighted_median(const double valores[], const double pesos[], int n) {
    if (n <= 0) {
        fprintf(stderr, "Erro: O vetor esta vazio.\n");
        exit(EXIT_FAILURE);
    }

    // Criar lista de pares (valor, peso) alocada dinamicamente
    elemento *pares = (elemento *)malloc(n * sizeof(elemento));
    if (pares == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        pares[i].valor = valores[i];
        pares[i].peso = pesos[i];
    }

    // ordena por valor porque queremos garantir que estamos acumulando
    // os pesos comecando pelos elementos menores, seguindo a definicao da
    // mediana ponderada (xi < xk)
    qsort(pares, n, sizeof(elemento), compare);

    double w_acumulado = 0.0;
    double mediana = pares[0].valor;

    // percorre acumulando os pesos
    for (int k = 0; k < n; k++) {
        w_acumulado += pares[k].peso;
        if (w_acumulado >= 0.5) {
            mediana = pares[k].valor;
            // como w_acumulado >= 0.5 ate xk = pares[i] (incluso)
            // a soma dos pesos apos xk eh menor que 0.5, pq 1-w_acumulado <= 0.5 
            // NOTE: como a soma dos pesos apos xk e antes de xk eh menor que 0.5
            // o elemento pares[i] eh a mediana ponderada, portanto, termine
            break;
        }
    }

    // libera a memoria alocada para os pares
    free(pares);
    return mediana;
}

#endif