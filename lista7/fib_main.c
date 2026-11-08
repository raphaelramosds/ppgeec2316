#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
    int x, y;
    if (n <= 1)
        return n;
    x = fib(n - 1);
    y = fib(n - 2);
    return x + y;
}

int main(int argc, char** argv)
{
    int n = (argc > 1) ? atoi(argv[1]) : 10;
    int r = -1;

    r = fib(n);
    
    printf("fib(%d) = %d", n, r);
    return 0;
}