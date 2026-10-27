#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// Custo de remover chaves presentes para diferentes fatores de carga
// (ciclos de remove + insere estao em test_hash_remove_insert.c)
// NOTE: encad_remover e aberto_remover recebem a chave, logo ambas fazem uma busca seguida de
// uma operacao O(1) (religar a lista / marcar DELETED)
#define L_ENCAD 16       // encadeamento: m = 2^16 = 65536
#define M_ABERTO 65537   // enderecamento aberto: 2^16 + 1 (primo)
#define PROB_LINEAR 1    // 1: sondagem linear; 0: hashing duplo

#define REPETICOES 10

int main() {
    rng_semear((uint64_t) time(NULL));

    double alphas[] = {0.1, 0.5, 0.9, 0.99, 1.0, 2.0, 5.0};
    int num_alphas = sizeof(alphas) / sizeof(alphas[0]);
    int m_encad = 1 << L_ENCAD;

    printf("remocao de 10%% das chaves presentes (escolhidas ao acaso)\n");
    printf("m = %d (encadeamento), %d (aberto, %s); media de %d tabelas por alpha\n\n",
           m_encad, M_ABERTO, PROB_LINEAR ? "linear" : "duplo", REPETICOES);
    printf("%-6s | %-16s %-16s | %-16s %-16s\n", "alpha",
           "encad_sondagens", "encad_ns", "aberto_sondagens", "aberto_ns");

    for (int t = 0; t < num_alphas; t++) {
        double a = alphas[t];
        int n_encad = (int) (a * m_encad);
        int n_aberto = (int) (a * M_ABERTO);
        int usa_aberto = a <= 1.0;
        medida encad = {0, 0}, abert = {0, 0};

        for (int r = 0; r < REPETICOES; r++) {
            int n_max = n_encad > n_aberto ? n_encad : n_aberto;
            int *A = gerar_chaves(n_max, 0);
            int *R = (int*) malloc(n_max * sizeof(int));
            if (R == NULL) {
                fprintf(stderr, "Erro de alocacao de memoria.\n");
                exit(1);
            }

            // NOTE: as chaves removidas R[0..q) sao sorteadas entre as presentes, e nao as primeiras
            // inseridas: na sondagem linear estas ficam na posicao de origem (custo ~1) e no
            // encadeamento ficam no fim das listas, o que enviesaria a comparacao
            hash_encadeamento *He = encad_criar(L_ENCAD);
            for (int i = 0; i < n_encad; i++) {
                encad_inserir(He, A[i]);
                R[i] = A[i];
            }
            embaralhar(R, n_encad);
            int q = n_encad / 10;
            long ok = 0;
            sondagens = 0;
            double inicio = agora_ns();
            for (int i = 0; i < q; i++) {
                ok += encad_remover(He, R[i]) != -1;
            }
            encad.ns += (agora_ns() - inicio) / q / REPETICOES;
            encad.sondagens += (double) sondagens / q / REPETICOES;
            encad_liberar(He);

            if (usa_aberto) {
                hash_aberto *Ha = aberto_criar(M_ABERTO, PROB_LINEAR);
                for (int i = 0; i < n_aberto; i++) {
                    aberto_inserir(Ha, A[i]);
                    R[i] = A[i];
                }
                embaralhar(R, n_aberto);
                q = n_aberto / 10;
                sondagens = 0;
                inicio = agora_ns();
                for (int i = 0; i < q; i++) {
                    ok += aberto_remover(Ha, R[i]) != -1;
                }
                abert.ns += (agora_ns() - inicio) / q / REPETICOES;
                abert.sondagens += (double) sondagens / q / REPETICOES;
                aberto_liberar(Ha);
            }

            sink = ok;
            free(A);
            free(R);
        }

        printf("%-6.2f | %-16.2f %-16.1f | ", a, encad.sondagens, encad.ns);
        if (usa_aberto) {
            printf("%-16.2f %-16.1f\n", abert.sondagens, abert.ns);
        } else {
            printf("%-16s %-16s\n", "n > m", "n > m");
        }
    }
    return 0;
}
