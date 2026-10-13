#ifndef SELECTIONRANDOM_H
#define SELECTIONRANDOM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

int randomized_partition(int A[], int p, int r) {
    // escolhe uma posicao entre p e r aleatoriamente
    int i = p + rand() % (r - p + 1);

    // coloca o pivo no final da particao
    swap(&A[i], &A[r]);

    // posiciona elementos menores que o pivo a esquerda e maiores a direita
    return partition(A, p, r);
}

int randomized_select(int A[], int p, int r, int i) {
    if (p == r) {
        // particao tem apenas um elemento
        return A[p];
    }

    // escolha o pivo
    int q = randomized_partition(A, p, r);

    // rank do pivo: k-esimo maior valor da particao esquerda
    int k = q - p + 1;

    if (i == k) {
        // calculamos um rank da posicao desejada (o "i") entao retorne
        return A[q];
    } else if (i < k) {
        // calculamos um rank maior do que o desejado, entao faca o mesmo
        // processo de selecao na particao esquerda
        return randomized_select(A, p, q - 1, i);
    } else {
        // calculamos um rank menor do que o desejado
        return randomized_select(A, q + 1, r, i - k);
    }
}

#endif