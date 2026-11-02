#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

void dijkstra(const grafo *G, int s, double *d, int *pi);

void imprimir_caminho(const grafo *G, const int *pi, int s, int v);

#endif