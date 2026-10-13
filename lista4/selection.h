#ifndef SELECTION_H
#define SELECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Encontre o i-esimo menor elemento em A[p:r] utilizando selecao aleatoria do pivo
int randomized_select(int A[], int p, int r, int i);

// Encontre o i-esimo menor elemento em A[p:r] utilizando a selecao do melhor pivo pela mediana das medianas dos grupos
int median_select(int A[], int p, int r, int i);

#endif