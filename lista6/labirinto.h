#ifndef LABIRINTO_H
#define LABIRINTO_H

#include "grafo.h"

typedef struct {
    int u, v;   // coordenada (vertice) de origem (u) e destino (v) da parede
    double w;   // peso da parede (aleatorio)
    int aberta; // parede aberta (0) ou fechada (1)
} parede;

typedef struct {
    int linhas, colunas;
    int n_paredes;
    parede *paredes;
} labirinto;

labirinto *labirinto_criar(int linhas, int colunas);

void labirinto_liberar(labirinto *L);

grafo *labirinto_grafo(const labirinto *L);

// abre parede (u,v) do labirinto: contabiliza como aresta valida na MST
void labirinto_abrir(labirinto *L, int u, int v);

int labirinto_ler_argumentos(int argc, char *argv[], int *linhas, int *colunas, unsigned *semente,
                             int *desenhar);

#endif
