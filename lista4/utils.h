#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

// troca elementos
static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// compare dois elementos: -1 (b > a), 0 (iguais), 1 (a > b)
static inline int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// preenche A com valores aleatorios
static inline void gerar_aleatorio(int A[], int n) {
    for (int k = 0; k < n; k++) {
        A[k] = rand();
    }
}

// preenche A com os valores de 1 a n em ordem crescente
static inline void gerar_ordenado(int A[], int n) {
    for (int k = 0; k < n; k++) {
        A[k] = k + 1;
    }
}

#endif
