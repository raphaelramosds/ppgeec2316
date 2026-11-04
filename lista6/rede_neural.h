#ifndef REDE_NEURAL_H
#define REDE_NEURAL_H

#include "grafo.h"

// monta o grafo nao dirigido de uma rede neural totalmente conectada:
// I : tamanho da entrada (vertices de entrada)
// W : numero de camadas ocultas
// n : numero de neuronios em cada camada (vertices em cada camada oculta)
// O : tamanho da saida (vertices de saida)
grafo *rede_neural_grafo(int I, int W, int n, int O);

#endif
