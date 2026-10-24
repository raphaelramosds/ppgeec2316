#include "array_ordenado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

array_ordenado *array_criar(void) {
    array_ordenado *a = (array_ordenado*) malloc(sizeof(array_ordenado));
    a->tamanho = 0;
    a->capacidade = 1;
    a->chaves = (int*) malloc(a->capacidade * sizeof(int));
    return a;
}

void array_liberar(array_ordenado *a) {
    free(a->chaves);
    free(a);
}

// use busca binaria para retornar o primeiro indice i tal que A[i] >= x (ou "tamanho" se todos forem menores)
static int limite_inferior(const array_ordenado *a, int x) {
    int baixo = 0, alto = a->tamanho;

    // invariante: A[0:baixo-1] < x e A[alto:tamanho-1] >= x
    while (baixo < alto) {
        int meio = baixo + (alto - baixo) / 2;
        if (a->chaves[meio] < x) {
            baixo = meio + 1;
        } else {
            alto = meio;
        }
    }
    return baixo;
}

void array_inserir(array_ordenado *a, int x) {
    // TABLE-INSERT: se a tabela esta cheia, dobra a capacidade
    // NOTE: o custo amortizado da expansao eh O(1) por insercao
    if (a->tamanho == a->capacidade) {
        a->capacidade *= 2;
        a->chaves = (int*) realloc(a->chaves, a->capacidade * sizeof(int));
        if (a->chaves == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria.\n");
            exit(1);
        }
    }

    int i = limite_inferior(a, x);

    // desloca A[i:tamanho-1] uma posicao a direita para abrir espaco para x
    // NOTE: este deslocamento domina o custo da insercao, Theta(n) no pior caso
    memmove(&a->chaves[i + 1], &a->chaves[i], (a->tamanho - i) * sizeof(int));
    a->chaves[i] = x;
    a->tamanho++;
}

int array_buscar(const array_ordenado *a, int x) {
    int i = limite_inferior(a, x);
    if (i < a->tamanho && a->chaves[i] == x) {
        return i;
    }
    return -1;
}
