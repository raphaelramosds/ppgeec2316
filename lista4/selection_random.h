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
    // NOTE: se ele for o maior/menor elemento da particao, a divisao fica desbalanceada
    // atingindo, o pior caso do algoritmo
    swap(&A[i], &A[r]);

    // posiciona elementos menores que o pivo a esquerda e maiores a direita
    return partition(A, p, r);
}

// seleciona o i-esimo menor valor em A[p:r]
int randomized_select(int A[], int p, int r, int i) {

    if (p == r) {
        // particao tem apenas um elemento
        return A[p];
    }

    // escolha o pivo aleatoriamente
    int q = randomized_partition(A, p, r);

    // rank de A[q]: k-esimo menor valor da particao esquerda
    // exemplo: [2,5,8,9,10], rank(5) = 3 elementos menores ou iguais a 5 
    int k = q - p + 1;

    if (i == k) {
        // calculamos um rank da posicao desejada (o "i") entao retorne
        return A[q];
    } else if (i < k) {
        // calculamos um rank maior do que o desejado, entao procure APENAS na particao esquerda
        return randomized_select(A, p, q - 1, i);
    } else {
        // calculamos um rank menor do que o desejado, entao procure APENAS na particao direita
        // NOTE: achar (i-k)-esimo menor porque perdemos a referencia da particao esquerda
        return randomized_select(A, q + 1, r, i - k);
    }
}

#endif