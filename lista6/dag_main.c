// grafo computacional (DAG) da expressao f(x, y) = x*y + sin(x), ordenado topologicamente com DFS
// uso: ./dag_main
#include "dfs.h"
#include "grafo.h"
#include <stdio.h>

// vertices do grafo
enum {
    X,       // valor x
    Y,       // valor y
    OP_SIN,  // operacao sin
    OP_MUL,  // operacao *
    SINX,    // valor sin(x)
    XY,      // valor x*y
    OP_SOMA, // operacao +
    F,       // valor f
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

    dfs_resultado *R = dfs(G);

    // cada vertice eh visitado duas vezes: uma para calcular o tempo inicial e uma para calcular o final
    // entao, o maior tempo final possivel eh de 2*(numero de vertices)
    int qtde_finais = 2*N_VERTICES;

    // como o primeiro tempo final eh zero, somamos 1
    int n_posicoes_finais = qtde_finais + 1;
    
    int vertices_expandido[n_posicoes_finais];

    // inicializacao do array ordenado
    // NOTE: colocamos -1 nas posicoes que nao correspondem a nenhum vertice no array expandido
    // NOTE: logica similar ao array de frequencias do counting sort
    for (int t = 0; t < n_posicoes_finais; t++) {
        vertices_expandido[t] = -1;
    }

    // o valor tempo de finalizacao f[u] corresponde a sua posicao ordenada no array de vertices
    for (int u = 0; u < N_VERTICES; u++) {
        vertices_expandido[R->f[u]] = u;
    }

    printf("Ordem topologica de f(x, y) = x*y + sin(x):\n");
    int i = 1;
    
    // percorra na ordem decrescente 
    for (int t = qtde_finais; t >= 1; t--) {
        if (vertices_expandido[t] != -1) {
            printf("  %d. %s\n", i++, NOME[vertices_expandido[t]]);
        }
    }

    dfs_liberar(R);
    grafo_liberar(G);
    return 0;
}
