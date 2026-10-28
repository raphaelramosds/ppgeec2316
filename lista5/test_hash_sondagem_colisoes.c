#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"

// Sondagem linear x hashing duplo no pior caso: n chaves com o mesmo h1
// As chaves k_j = j * m + C (j = 0, ..., n - 1) tem h1(k_j) = C para todo j:
// - sondagem linear: todas seguem a mesma sequencia C, C + 1, C + 2, ... (agrupamento
//   secundario), logo a j-esima insercao examina j + 1 posicoes e o total eh Theta(n^2)
// - hashing duplo: como m = 1 (mod m - 1), h2(k_j) = 1 + (j + C) mod (m - 1) eh diferente
//   para cada j e as sequencias divergem apos a primeira sondagem
#define M_ABERTO 65537 // 2^16 + 1 (primo)
#define C 12345        // posicao inicial comum (qualquer valor em [0, m))

// NOTE: k_j deve caber em int, logo n <= (2^31 - 1 - C) / m ~ 32767
#define N_MAX 32000

int main() {
    (void) gerar_chaves; // as chaves aqui sao construidas, nao aleatorias

    int ns[] = {1000, 2000, 4000, 8000, 16000, 32000};
    int num_ns = sizeof(ns) / sizeof(ns[0]);

    int *A = (int*) malloc(N_MAX * sizeof(int));
    if (A == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    for (int j = 0; j < N_MAX; j++) {
        A[j] = j * M_ABERTO + C;
    }

    printf("Para m = %d e chaves k_j = j * m + %d (todas com h1 = %d)\n\n", M_ABERTO, C, C);
    printf("Numero de Sondagens e Tempo gasto para Insercoes (ns)\n\n");
    printf("%-6s %-6s | %-12s %-12s | %-12s %-12s\n", "n", "alpha",
           "linear_sond", "linear_ns", "duplo_sond", "duplo_ns");

    for (int t = 0; t < num_ns; t++) {
        int n = ns[t];
        printf("%-6d %-6.3f |", n, (double) n / M_ABERTO);

        for (int e = 0; e < 2; e++) {
            hash_aberto *H = aberto_criar(M_ABERTO, e == 0); // e == 0: sondagem linear

            long ok = 0;
            sondagens = 0;
            double inicio = agora_ns();
            for (int j = 0; j < n; j++) {
                ok += aberto_inserir(H, A[j]) != -1;
            }
            medida ins = {(double) sondagens / n, (agora_ns() - inicio) / n};
            sink = ok;

            printf(" %-12.2f %-12.1f%s", ins.sondagens, ins.ns, e == 0 ? " |" : "\n");
            aberto_liberar(H);
        }
    }

    printf("\nsond: numero de sondagens (posicoes examinadas por insercao)\n");
    printf("ns: tempo por insercao (nanossegundos)\n");
    printf("linear: hashing por prob linear\n");
    printf("duplo: hashing duplo\n");

    free(A);
    return 0;
}
