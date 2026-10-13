#include "selection.h"
#include "utils.h"

// Roda o teste para cada tamanho, gerando o array com a funcao "gerar"
void executar_teste(const char *titulo, void (*gerar)(int[], int), const int tamanhos[], int num_tamanhos, int repeticoes) {
    printf("%s\n", titulo);
    printf("%-12s   %-20s   %-20s\n", "Tamanho (n)", "Randomized (ms)", "Median (ms)");

    for (int t = 0; t < num_tamanhos; t++) {
        int n = tamanhos[t];
        int *original = (int*) malloc(n * sizeof(int));

        gerar(original, n);

        // busca a mediana (i = n/2)
        int i = n / 2;
        double t_rand = medir_tempo(original, n, i, RANDOMIZED, repeticoes);
        double t_med = medir_tempo(original, n, i, MEDIANA, repeticoes);

        printf("%-12d   %-20.3f   %-20.3f\n", n, t_rand * 1000, t_med * 1000);

        free(original);
    }

    printf("\n");
}

int main() {
    // Inicializa a semente aleatoria
    srand((unsigned int)time(NULL));

    int tamanhos[] = {100000, 200000, 400000, 800000, 1600000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    int repeticoes = 10;

    // TESTE 1: o median select tem maior constante, entao pode apresentar tempos de execucao maiores
    executar_teste("TESTE 1: array aleatorio", gerar_aleatorio, tamanhos, num_tamanhos, repeticoes);

    // TESTE 2: tenta forcar o pior caso do randomized select com um array ordenado de 1 a n
    executar_teste("TESTE 2: array ordenado (1 a n)", gerar_ordenado, tamanhos, num_tamanhos, repeticoes);

    return 0;
}
