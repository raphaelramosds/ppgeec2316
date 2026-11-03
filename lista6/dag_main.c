// grafo computacional (DAG) da expressao f(x, y) = x*y + sin(x), ordenado topologicamente com DFS
// uso: ./dag_main
#include "dfs.h"
#include "grafo.h"
#include <stdio.h>

// vertices do grafo
enum {
    F,       // valor f
    OP_SOMA, // operacao +
    XY,      // valor x*y
    SINX,    // valor sin(x)
    OP_MUL,  // operacao *
    OP_SIN,  // operacao sin
    X,       // valor x
    Y,       // valor y
    N_VERTICES
};

static const char *NOME[N_VERTICES] = {
    [F]       = "f",
    [OP_SOMA] = "[+]",
    [XY]      = "x*y",
    [SINX]    = "sin(x)",
    [OP_MUL]  = "[*]",
    [OP_SIN]  = "[sin]",
    [X]       = "x",
    [Y]       = "y",
};

static grafo *montar_dag(void) {
    grafo *G = grafo_criar(N_VERTICES, DIRIGIDO);
    // x*y
    grafo_adicionar_aresta(G, X, OP_MUL, 1.0);
    grafo_adicionar_aresta(G, Y, OP_MUL, 1.0);
    grafo_adicionar_aresta(G, OP_MUL, XY, 1.0);
    // sin(x)
    grafo_adicionar_aresta(G, X, OP_SIN, 1.0);
    grafo_adicionar_aresta(G, OP_SIN, SINX, 1.0);
    // x*y + sin(x)
    grafo_adicionar_aresta(G, XY, OP_SOMA, 1.0);
    grafo_adicionar_aresta(G, SINX, OP_SOMA, 1.0);
    grafo_adicionar_aresta(G, OP_SOMA, F, 1.0);
    return G;
}

int main(void) {
    grafo *G = montar_dag();

    int ordem[N_VERTICES];
    if (ordenacao_topologica(G, ordem) != 0) {
        fprintf(stderr, "Erro: o grafo possui ciclo.\n");
        grafo_liberar(G);
        return 1;
    }

    printf("Ordem topologica de f(x, y) = x*y + sin(x):\n");
    for (int i = 0; i < N_VERTICES; i++) {
        printf("  %d. %s\n", i + 1, NOME[ordem[i]]);
    }

    grafo_liberar(G);
    return 0;
}
