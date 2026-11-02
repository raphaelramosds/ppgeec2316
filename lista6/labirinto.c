#include "labirinto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// monta labirinto M x N, em que M = linhas e N = colunas
// grid 3 x 3 possui 6 grades verticais e 6 horizontais
// +---+---+---+
// | 0 | 1 | 2 |     verticais:   3 * (3-1) = 6
// +---+---+---+     horizontais: (3-1) * 3 = 6
// | 3 | 4 | 5 |     total:       12
// +---+---+---+
// | 6 | 7 | 8 |
// +---+---+---+
//
// NOTE: na N-esima linha/coluna, temos apenas N-1 paredes
//
// neste grid, o vetor de paredes tem 12 posicoes
//         0     1     2     3     4     5    |   6     7     8     9    10    11
// (u,v): (0,1) (1,2) (3,4) (4,5) (6,7) (7,8) | (0,3) (1,4) (2,5) (3,6) (4,7) (5,8)
//        <---------- verticais ----------->  | <--------- horizontais ----------->
//
// NOTE: primeiro sao inseridas as coordenadas das paredes verticais, depois as horizontais
labirinto *labirinto_criar(int linhas, int colunas) {
    labirinto *L = (labirinto*) malloc(sizeof(labirinto));
    if (L == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    L->linhas = linhas;
    L->colunas = colunas;
    // numero de arestas eh o numero de paredes (divisorias) em uma grade M x N
    L->n_paredes = linhas * (colunas - 1) + (linhas - 1) * colunas; // M * (N - 1) + (M - 1) * N
    L->paredes = (parede*) malloc((L->n_paredes > 0 ? L->n_paredes : 1) * sizeof(parede));
    if (L->paredes == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }

    int k = 0;
    // paredes verticais: entre (r, c) e (r, c+1)
    for (int r = 0; r < linhas; r++) {
        for (int c = 0; c + 1 < colunas; c++) {
            int u = r * colunas + c; // coordenada no grid (row major)
            L->paredes[k++] = (parede) {u, u + 1, rand() / (RAND_MAX + 1.0), 0};
        }
    }
    // paredes horizontais: entre (r, c) e (r+1, c)
    for (int r = 0; r + 1 < linhas; r++) {
        for (int c = 0; c < colunas; c++) {
            int u = r * colunas + c; // coordenada no grid (row major)
            L->paredes[k++] = (parede) {u, u + colunas, rand() / (RAND_MAX + 1.0), 0};
        }
    }
    return L;
}

void labirinto_liberar(labirinto *L) {
    free(L->paredes);
    free(L);
}

grafo *labirinto_grafo(const labirinto *L) {
    grafo *G = grafo_criar(L->linhas * L->colunas, NAO_DIRIGIDO);
    for (int k = 0; k < L->n_paredes; k++) {
        grafo_adicionar_aresta(G, L->paredes[k].u, L->paredes[k].v, L->paredes[k].w);
    }
    return G;
}

// indice em L->paredes da parede entre as celulas vizinhas u e v
static int indice_parede(const labirinto *L, int u, int v) {
    if (u > v) {
        int t = u;
        u = v;
        v = t;
    }

    // linha (row) e coluna (col) no grid
    int r = u / L->colunas;
    int c = u % L->colunas;

    // parede vertical: separa celulas lado a lado
    if (v == u + 1) {
        return r * (L->colunas - 1) + c;
    }

    // parede horizontal: separa celulas que estao uma acima da outra
    // NOTE: as paredes horizontais estao APOS as paredes verticais no vetor L->paredes
    // entao, como a quantidade de paredes verticais em um grid M x N eh M * (N - 1)
    // somamos essa quantidade a r * N + c, onde M = L->linhas e N = L->colunas
    return L->linhas * (L->colunas - 1) + r * L->colunas + c;
}

void labirinto_abrir(labirinto *L, int u, int v) {
    L->paredes[indice_parede(L, u, v)].aberta = 1;
}

static int ler_inteiro_positivo(const char *s, int *x) {
    char *fim;
    // converte string para inteiro
    long v = strtol(s, &fim, 10);
    if (*s == '\0' || *fim != '\0' || v < 1 || v > 10000) {
        // overflow/underflow
        return -1;
    }
    *x = (int) v;
    return 0;
}

int labirinto_ler_argumentos(int argc, char *argv[], int *linhas, int *colunas, unsigned *semente,
                             int *desenhar) {
    int dim[2], n_dim = 0;
    *semente = (unsigned) time(NULL);
    *desenhar = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            // -d: desenha o labirinto
            *desenhar = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            // -s semente: pode aparecer em qualquer posicao
            char *fim;
            if (i + 1 >= argc) {
                return -1;
            }
            *semente = (unsigned) strtoul(argv[++i], &fim, 10);
            if (*argv[i] == '\0' || *fim != '\0') {
                return -1;
            }
        } else if (n_dim < 2 && ler_inteiro_positivo(argv[i], &dim[n_dim]) == 0) {
            n_dim++;
        } else {
            return -1;
        }
    }
    if (n_dim == 0) {
        return -1;
    }
    *linhas = dim[0];
    *colunas = (n_dim == 2) ? dim[1] : dim[0]; // so as linhas: grade quadrada
    return 0;
}