#ifndef SELECTIONMEDIAN_H
#define SELECTIONMEDIAN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

void insertion_sort(int A[], int p, int r) {
    for (int i = p + 1; i <= r; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= p && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

// Particao em torno de um pivo especifico x
int partition_around(int A[], int p, int r, int x) {
    int i;
    // Procura o valor x e o move para o final para usar como pivo
    for (i = p; i <= r; i++) {
        if (A[i] == x) {
            swap(&A[i], &A[r]);
            break;
        }
    }
    return partition(A, p, r);
}

// Prototipo
int median_select(int A[], int p, int r, int i);

int median_select(int A[], int p, int r, int i) {
    // Ajusta o tamanho para ser divisivel por 5 se necessario e isola minimos
    while ((r - p + 1) % 5 != 0) {
        for (int j = p + 1; j <= r; j++) {
            if (A[p] > A[j]) {
                swap(&A[p], &A[j]);
            }
        }
        if (i == 1) return A[p];
        p++;
        i--;
    }

    int g = (r - p + 1) / 5;

    // Ordena cada grupo de 5 elementos e coloca as medianas no inicio
    for (int j = 0; j < g; j++) {
        int sub_p = p + j * 5;
        int sub_r = sub_p + 4;
        insertion_sort(A, sub_p, sub_r);
        // Move a mediana do grupo (sub_p + 2) para a regiao inicial dos medianos
        swap(&A[p + j], &A[sub_p + 2]);
    }

    // Encontra recursivamente a mediana das medianas x
    int x = median_select(A, p, p + g - 1, g / 2);

    // Particiona o vetor em torno de x
    int q = partition_around(A, p, r, x);
    int k = q - p + 1;

    // Comportamento similar ao randomized-select
    if (i == k) {
        return A[q];
    } else if (i < k) {
        return median_select(A, p, q - 1, i);
    } else {
        return median_select(A, q + 1, r, i - k);
    }
}

#endif