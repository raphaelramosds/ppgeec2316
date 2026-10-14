#include "minmax.h"
#include <time.h>

// Funcao de medicao que repete o teste e retorna o menor tempo obtido
double medir(const double *a, int n, int repeticoes, int simultaneo) {
    clock_t inicio, fim;
    double menor_tempo = -1.0;

    // force o compilador a reler o ponteiro a cada repeticao, impedindo que a chamada seja movida para fora do laco
    const double *volatile vetor = a;
    
    // evita eliminacao de codigo morto (forca o uso dos resultados de minmax_simultaneo e minmax_nao_simultaneo)
    volatile double sink_min, sink_max;

    for (int r = 0; r < repeticoes; r++) {
        inicio = clock();

        resultado res;
        if (simultaneo) {
            res = minmax_simultaneo(vetor, n);
        } else {
            res = minmax_nao_simultaneo(vetor, n);
        }
        sink_min = res.min_val;
        sink_max = res.max_val;

        fim = clock();
        double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (menor_tempo < 0 || tempo_gasto < menor_tempo) {
            menor_tempo = tempo_gasto;
        }
    }
    (void)sink_min;
    (void)sink_max;

    return menor_tempo;
}

int main() {
    // Inicializa a semente aleatoria
    srand((unsigned int)time(NULL));

    int tamanhos[] = {10e6, 2*10e6, 4*10e6, 8*10e6, 16*10e6};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    int repeticoes = 50;

    printf("%-12s   %-15s   %-18s \n", "Tamanho (n)", "Simultaneo (ms)", "Nao-Simultaneo (ms)");
    // printf("-----------------------------------------------------------------\n");

    for (int t = 0; t < num_tamanhos; t++) {
        int n = tamanhos[t];
        
        // Aloca o vetor dinamicamente para suportar valores grandes de n
        double *v = (double *)malloc(n * sizeof(double));
        if (v == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria.\n");
            return 1;
        }

        // Preenche com valores aleatorios
        for (int i = 0; i < n; i++) {
            v[i] = rand() % 10000001;
        }

        double tempo_sim = medir(v, n, repeticoes, 1) * 1000; // ms
        double tempo_nao_sim = medir(v, n, repeticoes, 0) * 1000; // ms

        printf("%-12d   %-15.6f   %-18.6f\n", n, tempo_sim, tempo_nao_sim);

        free(v); // Libera a memoria alocada
    }

    return 0;
}