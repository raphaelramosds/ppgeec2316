// tabela de roteamento OSPF calculada com Dijkstra sobre um LSDB simplificado
// uso: ./ospf_main [roteador_origem]   (padrao: 0)
#include "dijkstra.h"
#include "grafo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BANDA_REFERENCIA_MBPS 10000.0
#define MAX_LINKS 4

// link de um LSA (aresta)
typedef struct {
    int vizinho;    // roteador na outra ponta do link
    int banda_mbps; // largura de banda do link
} link_ospf;

// LSA de um roteador (lista de adjacencia de um roteador)
typedef struct {
    // vertice
    int roteador;
    // qtde atual de links
    int n_links;
    // lista de adjacencia
    link_ospf links[MAX_LINKS];
} lsa;

enum { R0, R1, R2, R3, R4, R5, N_ROTEADORES }; // 6 roteadores

static const lsa LSDB[N_ROTEADORES] = {
    // roteador, n_links, links
    {R0, 2, {{R1, 1000}, {R2, 100}}},
    {R1, 3, {{R0, 1000}, {R2, 1000}, {R3, 100}}},
    {R2, 4, {{R0, 100}, {R1, 1000}, {R3, 10000}, {R4, 1000}}},
    {R3, 4, {{R1, 100}, {R2, 10000}, {R4, 1000}, {R5, 1000}}},
    {R4, 3, {{R2, 1000}, {R3, 1000}, {R5, 100}}},
    {R5, 2, {{R3, 1000}, {R4, 100}}},
};

// monta o grafo dirigido da topologia
static grafo *topologia_do_lsdb(const lsa *lsdb, int n_roteadores) {
    grafo *G = grafo_criar(n_roteadores, DIRIGIDO);
    for (int i = 0; i < n_roteadores; i++) {
        // adicione todos os vizinhos (links) deste roteador no grafo
        for (int j = 0; j < lsdb[i].n_links; j++) {
            const link_ospf *l = &lsdb[i].links[j];
            grafo_adicionar_aresta(G, lsdb[i].roteador, l->vizinho, BANDA_REFERENCIA_MBPS/l->banda_mbps);
        }
    }
    return G;
}

int main(int argc, char *argv[]) {
    // roteador de origem
    int s = (argc > 1) ? atoi(argv[1]) : R0;
    if (s < 0 || s >= N_ROTEADORES) {
        fprintf(stderr, "roteador de origem deve estar entre 0 e %d\n", N_ROTEADORES - 1);
        return 1;
    }

    // montar topologia
    grafo *G = topologia_do_lsdb(LSDB, N_ROTEADORES);

    double d[N_ROTEADORES]; // pesos totais minimos
    int pi[N_ROTEADORES];   // vertice anterior do n-esimo roteador

    // calcular tabela de roteamendo
    dijkstra(G, s, d, pi);

    printf("Tabela de roteamento de R%d (referencia %.0f Mbps)\n\n", s, BANDA_REFERENCIA_MBPS);
    printf("%-8s %-6s %s\n", "destino", "custo", "caminho");
    for (int v = 0; v < N_ROTEADORES; v++) {
        if (v == s) {
            continue;
        }
        printf("R%-7d ", v);

        // imprimir menor custo ate v
        if (isinf(d[v])) {
            printf("%-6s ", "inf");
        } else {
            printf("%-6g ", d[v]);
        }

        // imprimir caminho de s ate v
        imprimir_caminho(G, pi, s, v);

        printf("\n");
    }

    grafo_liberar(G);
    return 0;
}
