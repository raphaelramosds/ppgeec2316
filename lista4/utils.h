#ifndef UTILS_H
#define UTILS_H

static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static inline int partition(int A[], int p, int r) {
    // pivo
    int x = A[r];

    // representa o ultimo indice da particao esquerda
    int i = p - 1;

    for (int j = p; j < r; j++) {
        // troca se achar um elemento menor que o pivo
        // para posiciona-lo na particao esquerda (a particao com os menores elementos)
        if (A[j] <= x) {
            i++;
            swap(&A[i], &A[j]);
        }
    }

    // coloca pivo na posicao correta
    // NOTE: "i + 1" se encontra apos o ultimo indice "i" da particao esquerda 
    swap(&A[i + 1], &A[r]);

    return i + 1;
}

#endif
