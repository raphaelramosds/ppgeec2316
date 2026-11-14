#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "pascalops.h"

// indexacao row-major: i * N_COLUNAS + j
#define EL(m, n, i, j) ((m)[(size_t)(i) * (n) + (j)])

// abaixo deste tamanho a recursao segue sequencial (evita overhead de criar tasks minusculas)
#ifndef CUTOFF
#define CUTOFF 64
#endif

// versao sequencial
static void multm_seq(const double *a, const double *b, double *c, int ld,
                      int linha_a, int col_a, int linha_b, int col_b,
                      int linha_c, int col_c, int n)
{
    // caso base: submatrizes 1x1 (acumula o produto direto em C)
    if (n == 1)
    {
        EL(c, ld, linha_c, col_c) += EL(a, ld, linha_a, col_a) * EL(b, ld, linha_b, col_b);
        return;
    }

    int meio = n / 2;

    multm_seq(a, b, c, ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio);
    multm_seq(a, b, c, ld, linha_a, col_a + meio, linha_b + meio, col_b, linha_c, col_c, meio);
    multm_seq(a, b, c, ld, linha_a, col_a, linha_b, col_b + meio, linha_c, col_c + meio, meio);
    multm_seq(a, b, c, ld, linha_a, col_a + meio, linha_b + meio, col_b + meio, linha_c, col_c + meio, meio);
    multm_seq(a, b, c, ld, linha_a + meio, col_a, linha_b, col_b, linha_c + meio, col_c, meio);
    multm_seq(a, b, c, ld, linha_a + meio, col_a + meio, linha_b + meio, col_b, linha_c + meio, col_c, meio);
    multm_seq(a, b, c, ld, linha_a + meio, col_a, linha_b, col_b + meio, linha_c + meio, col_c + meio, meio);
    multm_seq(a, b, c, ld, linha_a + meio, col_a + meio, linha_b + meio, col_b + meio, linha_c + meio, col_c + meio, meio);
}

static void _multm_recursivo(const double *a, const double *b, double *c, int ld,
                             int linha_a, int col_a, int linha_b, int col_b,
                             int linha_c, int col_c, int n)
{
    if (n <= CUTOFF)
    {
        multm_seq(a, b, c, ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, n);
        return;
    }

    int meio = n / 2;

    // NOTE:
    //   shared       -> a, b, c: todas as tasks acessam as mesmas matrizes
    //   firstprivate -> ld, indices e meio: cada task copia os valores do momento
    //                   em que foi criada (se usasse private, esses valores nao eram copiados)
    
    // fase 1: Cij = Ai1 * B1j
    // c11 = A11*B11
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio);
    // c12 = A11*B12
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a, col_a, linha_b, col_b + meio, linha_c, col_c + meio, meio);
    // c21 = A21*B11
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a + meio, col_a, linha_b, col_b, linha_c + meio, col_c, meio);
    // c22 = A21*B12 (executada pela propria thread, que de outra forma ficaria ociosa)
    _multm_recursivo(a, b, c, ld, linha_a + meio, col_a, linha_b, col_b + meio, linha_c + meio, col_c + meio, meio);

// espere os primeiros produtos finalizarem e evita condicao de corrida ao acumular soma na matriz c
#pragma omp taskwait

    // fase 2: Cij += Ai2 * B2j
    // c11 += A12*B21
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a, col_a + meio, linha_b + meio, col_b, linha_c, col_c, meio);
    // c12 += A12*B22
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a, col_a + meio, linha_b + meio, col_b + meio, linha_c, col_c + meio, meio);
    // c21 += A22*B21
#pragma omp task default(none) shared(a, b, c) \
    firstprivate(ld, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio)
    _multm_recursivo(a, b, c, ld, linha_a + meio, col_a + meio, linha_b + meio, col_b, linha_c + meio, col_c, meio);
    // c22 += A22*B22
    _multm_recursivo(a, b, c, ld, linha_a + meio, col_a + meio, linha_b + meio, col_b + meio, linha_c + meio, col_c + meio, meio);

// espere a conclusao dos produtos da fase 2
#pragma omp taskwait
}

// multiplicacao recursiva entre matrizes quadradas A e B
// NOTE: inicializar double *c com zeros
void multm_recursivo(const double *a, const double *b, double *c, int n)
{
    pascal_start(1);
#pragma omp parallel default(none) shared(a, b, c, n)
#pragma omp single
    _multm_recursivo(a, b, c, n, 0, 0, 0, 0, 0, 0, n);
    pascal_stop(1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "uso: %s <n>\n", argv[0]);
        fprintf(stderr, "  n  dimensao das matrizes quadradas (potencia de 2)\n");
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    // a divisao em quadrantes assume n potencia de 2
    if (n <= 0 || (n & (n - 1)) != 0)
    {
        fprintf(stderr, "n deve ser potencia de 2 (n=%d)\n", n);
        return EXIT_FAILURE;
    }

    // inicializar entradas
    double *a = malloc((size_t)n * n * sizeof(double));
    double *b = malloc((size_t)n * n * sizeof(double));
    if (a == NULL || b == NULL)
    {
        fprintf(stderr, "Falha ao alocar matrizes de entrada\n");
        return EXIT_FAILURE;
    }

    // semente fixa do gerador, para que A e B sejam as mesmas em toda execucao
    srand48(0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            EL(a, n, i, j) = drand48();
            EL(b, n, i, j) = drand48();
        }
    }

    // inicializar saida
    double *c = calloc((size_t)n * n, sizeof(double));
    if (c == NULL)
    {
        fprintf(stderr, "Falha ao alocar matriz %dx%d\n", n, n);
        exit(EXIT_FAILURE);
    }

    // multiplicar matrizes
    double inicio = omp_get_wtime();
    multm_recursivo(a, b, c, n);
    double fim = omp_get_wtime();
    double ts_ms = (fim - inicio) * 1000.0;

    printf("Recursive C=A*B (n=%d) took %.6f ms\n", n, ts_ms);

    free(a);
    free(b);
    free(c);
    return 0;
}
