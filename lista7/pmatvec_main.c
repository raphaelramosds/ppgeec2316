#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void p_mat_vec(const double *A, const double *x, double *y, int n)
{
    int i, j;

    // default(none): o compilador exige que toda variavel usada na regiao seja classificada explicitamente
    //
    // shared:
    // A, x, n -> apenas lidos por todas as threads
    // y -> escrito, mas cada iteracao i acessa somente y[i] (sem condicao de corrida)
    //
    // private:
    // i -> indice do laco paralelo: cada thread percorre o seu subconjunto de linhas
    // j -> cada thread precisa do seu proprio contador no laco interno
#pragma omp parallel for default(none) shared(A, x, y, n) private(i, j)
    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;
        for (j = 0; j < n; j++)
            y[i] = y[i] + A[i * n + j] * x[j];
    }
}

int main(int argc, char **argv)
{
    int n = (argc > 1) ? atoi(argv[1]) : 10;

    double *A = malloc((size_t)n * n * sizeof(double));
    double *x = malloc((size_t)n * sizeof(double));
    double *y = malloc((size_t)n * sizeof(double));

    for (int i = 0; i < n; i++)
    {
        x[i] = 1.0;
        for (int j = 0; j < n; j++)
            A[i * n + j] = i + j;
    }

    p_mat_vec(A, x, y, n);

    for (int i = 0; i < n && i < 10; i++)
        printf("y[%d] = %g\n", i, y[i]);

    free(A);
    free(x);
    free(y);
    return 0;
}
