#include "selection.h"
#include "utils.h"
#include <string.h>

void imprimir(const int A[], int n) {
    printf("[");
    for (int k = 0; k < n; k++) {
        printf("%d%s", A[k], k < n - 1 ? ", " : "");
    }
    printf("]\n");
}

int main() {
    // Inicializa a semente aleatoria (usada pelo randomized_select)
    srand((unsigned int)time(NULL));

    int lista[] = {12, 3, 5, 7, 4, 19, 26, 3, 8, 15, 1, 22, 10, 6, 17, 9, 2};
    int n = sizeof(lista) / sizeof(lista[0]);

    // referencia: lista ordenada, o i-esimo menor esta em ordenada[i-1]
    int ordenada[n];
    memcpy(ordenada, lista, sizeof(lista));
    qsort(ordenada, n, sizeof(int), compare);

    printf("Lista:    ");
    imprimir(lista, n);
    printf("Ordenada: ");
    imprimir(ordenada, n);
    printf("\n");

    printf("%-4s | %-10s | %-17s | %-13s\n", "i", "esperado", "randomized_select", "median_select");

    int falhas = 0;
    int copia[n];

    for (int i = 1; i <= n; i++) {
        // os algoritmos modificam o vetor, entao cada chamada recebe uma copia nova
        memcpy(copia, lista, sizeof(lista));
        int r_rand = randomized_select(copia, 0, n - 1, i);

        memcpy(copia, lista, sizeof(lista));
        int r_med = median_select(copia, 0, n - 1, i);

        int esperado = ordenada[i - 1];
        int ok = (r_rand == esperado) && (r_med == esperado);
        if (!ok) falhas++;

        printf("%-4d | %-10d | %-17d | %-13d %s\n", i, esperado, r_rand, r_med, ok ? "" : "<- ERRO");
    }

    printf("\n%s (%d falha(s))\n", falhas == 0 ? "Todos os testes passaram" : "Ha testes falhando", falhas);

    return falhas != 0;
}
