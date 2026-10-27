#ifndef TEST_HASH_UTILS_H
#define TEST_HASH_UTILS_H

// Utilitarios comuns aos experimentos com tabelas hash (test_hash_*.c)
// NOTE: deve ser o primeiro include, pois clock_gettime exige _POSIX_C_SOURCE com -std=c99
#define _POSIX_C_SOURCE 199309L

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// evita eliminacao de codigo morto (forca o uso dos resultados das operacoes)
static volatile long sink;

// gerador xorshift64: rand() tem RAND_MAX pequeno em algumas plataformas
static uint64_t rng_estado = 88172645463325252ull;

static inline void rng_semear(uint64_t s) {
    rng_estado = s != 0 ? s : 88172645463325252ull;
}

static inline uint64_t rng_proximo(void) {
    rng_estado ^= rng_estado << 13;
    rng_estado ^= rng_estado >> 7;
    rng_estado ^= rng_estado << 17;
    return rng_estado;
}

// chave aleatoria em [0, 2^30) com a paridade dada
// NOTE: as chaves presentes sao pares e as ausentes impares, logo toda busca por uma chave
// impar eh garantidamente sem sucesso
static inline int chave_aleatoria(int paridade) {
    return (int) (rng_proximo() % (1u << 29)) * 2 + paridade;
}

static int comparar_int(const void *a, const void *b) {
    int x = *(const int*) a, y = *(const int*) b;
    return (x > y) - (x < y);
}

// Fisher-Yates: permutacao aleatoria uniforme de A[0..n)
static void embaralhar(int A[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = (int) (rng_proximo() % (uint64_t) (i + 1));
        int t = A[i];
        A[i] = A[j];
        A[j] = t;
    }
}

// Retorna n chaves aleatorias distintas com a paridade dada, em ordem aleatoria
static int *gerar_chaves(int n, int paridade) {
    int *A = (int*) malloc(n * sizeof(int));
    if (A == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        A[i] = chave_aleatoria(paridade);
    }

    // as tabelas assumem que k ainda nao esta presente: substitui repetidas ate nao haver nenhuma
    int repetidas = 1;
    while (repetidas) {
        repetidas = 0;
        qsort(A, n, sizeof(int), comparar_int);
        for (int i = 1; i < n; i++) {
            if (A[i] == A[i - 1]) {
                A[i] = chave_aleatoria(paridade);
                repetidas = 1;
            }
        }
    }

    embaralhar(A, n); // desfaz a ordenacao
    return A;
}

// relogio monotonico em nanossegundos (clock() nao tem resolucao para operacoes de ~10 ns)
static inline double agora_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double) t.tv_sec * 1e9 + (double) t.tv_nsec;
}

typedef struct {
    double sondagens; // media por operacao
    double ns;        // tempo medio por operacao
} medida;

// Busca as q chaves de B e retorna a media de sondagens e de tempo por busca
static inline medida buscar_encad(const hash_encadeamento *H, const int B[], int q) {
    long encontrados = 0;
    sondagens = 0;
    double inicio = agora_ns();
    for (int i = 0; i < q; i++) {
        encontrados += encad_buscar(H, B[i]) != NULL;
    }
    double fim = agora_ns();
    sink = encontrados;
    return (medida) {(double) sondagens / q, (fim - inicio) / q};
}

static inline medida buscar_aberto(const hash_aberto *H, const int B[], int q) {
    long encontrados = 0;
    sondagens = 0;
    double inicio = agora_ns();
    for (int i = 0; i < q; i++) {
        encontrados += aberto_buscar(H, B[i]) != -1;
    }
    double fim = agora_ns();
    sink = encontrados;
    return (medida) {(double) sondagens / q, (fim - inicio) / q};
}

#endif
