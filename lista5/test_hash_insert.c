#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// Sondagens para inserir com a tabela ja em um fator de carga alpha: cada tabela eh preenchida
// ate alpha (sem medir) e entao mede-se a insercao de um lote pequeno de chaves novas
#define L_ENCAD 16       // encadeamento: m = 2^16 = 65536
#define M_ABERTO 65537   // enderecamento aberto: 2^16 + 1 (primo)
#define PROB_LINEAR 1    // 1: sondagem linear; 0: hashing duplo

#define REPETICOES 20
#define LOTE (M_ABERTO / 200) // ~0.5% de m: alpha praticamente nao muda durante o lote

int main() {
    rng_semear((uint64_t) time(NULL));

    double alphas[] = {0.1, 0.25, 0.5, 0.75, 0.9, 0.95, 0.99};
    int num_alphas = sizeof(alphas) / sizeof(alphas[0]);
    int m_encad = 1 << L_ENCAD;

    printf("m = %d (encadeamento), %d (aberto, %s); lote de %d insercoes, media de %d tabelas\n",
           m_encad, M_ABERTO, PROB_LINEAR ? "linear" : "duplo", LOTE, REPETICOES);
    printf("sondagens: nos examinados (encadeamento, insere na cabeca da lista) / posicoes examinadas (aberto)\n\n");
    printf("%-6s | %-16s | %-16s\n", "alpha", "encad_sondagens", "aberto_sondagens");

    for (int t = 0; t < num_alphas; t++) {
        double a = alphas[t];
        int n_encad = (int) (a * m_encad); // alpha = n/m entao n = alpha * m
        int n_aberto = (int) (a * M_ABERTO);
        int n_max = n_encad > n_aberto ? n_encad : n_aberto;
        double encad = 0, abert = 0; // sondagens medias por insercao

        for (int r = 0; r < REPETICOES; r++) {
            // preenchemos ate atingir o fator de carga
            int *A = gerar_chaves(n_max + LOTE, 0);
            const int *lote = A + n_max;

            hash_encadeamento *He = encad_criar(L_ENCAD);
            for (int i = 0; i < n_encad; i++) {
                encad_inserir(He, A[i]);
            }

            // comecar a medicao com o fator de carga estabelecido: media das sondagens no lote,
            // depois a media em relacao as repeticoes feitas
            sondagens = 0;
            for (int i = 0; i < LOTE; i++) {
                encad_inserir(He, lote[i]);
            }
            encad += (double) sondagens / LOTE / REPETICOES;
            encad_liberar(He);

            hash_aberto *Ha = aberto_criar(M_ABERTO, PROB_LINEAR);
            for (int i = 0; i < n_aberto; i++) {
                aberto_inserir(Ha, A[i]);
            }
            sondagens = 0;
            for (int i = 0; i < LOTE; i++) {
                aberto_inserir(Ha, lote[i]);
            }
            abert += (double) sondagens / LOTE / REPETICOES;
            aberto_liberar(Ha);

            free(A);
        }

        printf("%-6.2f | %-16.2f | %-16.2f\n", a, encad, abert);
    }
    return 0;
}
