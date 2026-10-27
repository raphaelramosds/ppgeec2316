#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// Busca com e sem sucesso para diferentes fatores de carga alpha = n/m, com m fixo
#define L_ENCAD 10      // encadeamento: m = 2^10 = 1024
#define M_ABERTO 1021   // enderecamento aberto: primo mais proximo de 1024
#define PROB_LINEAR 1   // 1: sondagem linear; 0: hashing duplo

#define REPETICOES 100  // tabelas construidas (com chaves novas) para cada alpha
#define NUM_FALHAS 1000 // buscas sem sucesso por tabela

int main() {
    rng_semear((uint64_t) time(NULL));

    double alphas[] = {0.1, 0.25, 0.5, 0.75, 0.9, 0.95, 0.99, 1.0, 2.0, 5.0};
    int num_alphas = sizeof(alphas) / sizeof(alphas[0]);
    int m_encad = 1 << L_ENCAD;
    const char *aberto = PROB_LINEAR ? "linear" : "duplo";

    // [0]: com sucesso, [1]: sem sucesso
    medida encad[num_alphas][2], abert[num_alphas][2];

    for (int t = 0; t < num_alphas; t++) {
        double a = alphas[t];
        int n_encad = (int) (a * m_encad);
        int n_aberto = (int) (a * M_ABERTO);
        int usa_aberto = a <= 1.0; // o enderecamento aberto nao comporta n > m

        for (int s = 0; s < 2; s++) {
            encad[t][s] = abert[t][s] = (medida) {0, 0};
        }

        for (int r = 0; r < REPETICOES; r++) {
            // as mesmas chaves (pares) nas duas tabelas e as mesmas chaves ausentes (impares)
            int *A = gerar_chaves(n_encad > n_aberto ? n_encad : n_aberto, 0);
            int *B = gerar_chaves(NUM_FALHAS, 1);

            hash_encadeamento *He = encad_criar(L_ENCAD);
            for (int i = 0; i < n_encad; i++) {
                encad_inserir(He, A[i]);
            }
            medida ok = buscar_encad(He, A, n_encad), falha = buscar_encad(He, B, NUM_FALHAS);
            encad[t][0].sondagens += ok.sondagens / REPETICOES;
            encad[t][0].ns += ok.ns / REPETICOES;
            encad[t][1].sondagens += falha.sondagens / REPETICOES;
            encad[t][1].ns += falha.ns / REPETICOES;
            encad_liberar(He);

            if (usa_aberto) {
                hash_aberto *Ha = aberto_criar(M_ABERTO, PROB_LINEAR);
                for (int i = 0; i < n_aberto; i++) {
                    aberto_inserir(Ha, A[i]);
                }
                ok = buscar_aberto(Ha, A, n_aberto);
                falha = buscar_aberto(Ha, B, NUM_FALHAS);
                abert[t][0].sondagens += ok.sondagens / REPETICOES;
                abert[t][0].ns += ok.ns / REPETICOES;
                abert[t][1].sondagens += falha.sondagens / REPETICOES;
                abert[t][1].ns += falha.ns / REPETICOES;
                aberto_liberar(Ha);
            }

            free(A);
            free(B);
        }
    }

    printf("m = %d (encadeamento), %d (aberto, %s); media de %d tabelas por alpha\n",
           m_encad, M_ABERTO, aberto, REPETICOES);
    printf("sondagens: nos examinados (encadeamento) / posicoes examinadas (aberto)\n\n");
    printf("%-6s | %-12s %-12s | %-12s %-12s\n", "alpha",
           "encad_ok", "encad_falha", "aberto_ok", "aberto_falha");
    for (int t = 0; t < num_alphas; t++) {
        printf("%-6.2f | %-12.2f %-12.2f | ", alphas[t],
               encad[t][0].sondagens, encad[t][1].sondagens);
        if (alphas[t] <= 1.0) {
            printf("%-12.2f %-12.2f\n", abert[t][0].sondagens, abert[t][1].sondagens);
        } else {
            printf("%-12s %-12s\n", "n > m", "n > m");
        }
    }

    printf("\ntempo medio por busca (ns)\n\n");
    printf("%-6s | %-12s %-12s | %-12s %-12s\n", "alpha",
           "encad_ok", "encad_falha", "aberto_ok", "aberto_falha");
    for (int t = 0; t < num_alphas; t++) {
        printf("%-6.2f | %-12.1f %-12.1f | ", alphas[t], encad[t][0].ns, encad[t][1].ns);
        if (alphas[t] <= 1.0) {
            printf("%-12.1f %-12.1f\n", abert[t][0].ns, abert[t][1].ns);
        } else {
            printf("%-12s %-12s\n", "n > m", "n > m");
        }
    }
    return 0;
}
