#include "array_ordenado.h"
#include "lista_encadeada.h"
#include "rubro_negra.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY 0
#define LISTA 1
#define ARVORE 2

// numero de buscas realizadas para cada n (metade com chaves presentes, metade aleatorias)
#define NUM_BUSCAS 2000

// evita eliminacao de codigo morto (forca o uso dos resultados das buscas)
static volatile long sink;

// Insere as n chaves de A na estrutura e retorna o tempo total gasto (s)
static double medir_insercao(int estrutura, void *e, const int A[], int n) {
    clock_t inicio = clock();
    for (int k = 0; k < n; k++) {
        if (estrutura == ARRAY) {
            array_inserir((array_ordenado*) e, A[k]);
        } else if (estrutura == LISTA) {
            lista_inserir((lista_encadeada*) e, A[k]);
        } else {
            rn_inserir((arvore_rn*) e, A[k]);
        }
    }
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// Busca as m chaves de B na estrutura e retorna o tempo medio de uma busca (s)
static double medir_busca(int estrutura, void *e, const int B[], int m) {
    long encontrados = 0;

    clock_t inicio = clock();
    for (int k = 0; k < m; k++) {
        if (estrutura == ARRAY) {
            encontrados += array_buscar((array_ordenado*) e, B[k]) != -1;
        } else if (estrutura == LISTA) {
            encontrados += lista_buscar((lista_encadeada*) e, B[k]) != NULL;
        } else {
            arvore_rn *T = (arvore_rn*) e;
            encontrados += rn_buscar(T, B[k]) != T->nil;
        }
    }
    clock_t fim = clock();

    sink = encontrados;
    return (double)(fim - inicio) / CLOCKS_PER_SEC / m;
}

int main() {
    srand((unsigned int)time(NULL));

    int tamanhos[] = {100000, 200000, 400000, 800000, 1000000, 2000000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("*_inserir: tempo total para inserir n chaves aleatorias (ms)\n");
    printf("*_buscar: tempo medio de uma busca sobre %d chaves (us)\n\n", NUM_BUSCAS);
    printf("%-10s | %-20s %-20s %-20s | %-20s %-20s %-20s\n", "n",
           "array_inserir", "lista_inserir", "rn_inserir",
           "array_buscar", "lista_buscar", "rn_buscar");

    for (int t = 0; t < num_tamanhos; t++) {
        int n = tamanhos[t];
        int *A = (int*) malloc(n * sizeof(int));
        int *B = (int*) malloc(NUM_BUSCAS * sizeof(int));
        if (A == NULL || B == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria.\n");
            return 1;
        }

        // as mesmas chaves sao inseridas, na mesma ordem, nas tres estruturas
        for (int k = 0; k < n; k++) {
            A[k] = rand();
        }
        // metade das buscas eh por chaves presentes (sucesso) e metade por chaves aleatorias
        // (quase sempre ausentes, que eh o pior caso para a lista)
        for (int k = 0; k < NUM_BUSCAS; k++) {
            B[k] = (k % 2 == 0) ? A[rand() % n] : rand();
        }

        double ins[3], busca[3];
        for (int estrutura = ARRAY; estrutura <= ARVORE; estrutura++) {
            void *e;
            if (estrutura == ARRAY) {
                e = array_criar();
            } else if (estrutura == LISTA) {
                e = lista_criar();
            } else {
                e = rn_criar();
            }

            ins[estrutura] = medir_insercao(estrutura, e, A, n) * 1e3;     // ms
            busca[estrutura] = medir_busca(estrutura, e, B, NUM_BUSCAS) * 1e6; // us

            if (estrutura == ARRAY) {
                array_liberar((array_ordenado*) e);
            } else if (estrutura == LISTA) {
                lista_liberar((lista_encadeada*) e);
            } else {
                rn_liberar((arvore_rn*) e);
            }
        }

        printf("%-10d | %-20.3f %-20.3f %-20.3f | %-20.4f %-20.4f %-20.4f\n", n,
               ins[ARRAY], ins[LISTA], ins[ARVORE], busca[ARRAY], busca[LISTA], busca[ARVORE]);

        free(A);
        free(B);
    }
    return 0;
}
