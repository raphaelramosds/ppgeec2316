#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
    int x, y;
    if (n <= 1)
        return n;
#pragma omp task shared(x) // spawn: pode despachar o calculo de x para outra thread
    x = fib(n - 1);
    y = fib(n - 2);
#pragma omp taskwait // sync: a thread atual DEVE esperar a conclusao da linha 11 para retornar o resultado
    return x + y;
}

int main(int argc, char** argv)
{
    int n = (argc > 1) ? atoi(argv[1]) : 10;
    int r = -1;

#pragma omp parallel // solicita a criacao da equipe de threads
#pragma omp single  // APENAS UMA thread pode executar o comando r=fib(n)
    r = fib(n);

    printf("fib(%d) = %d", n, r);
    return 0;
}