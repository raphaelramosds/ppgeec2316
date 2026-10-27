#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// Custo de inserir com a tabela ja em um fator de carga alpha: cada tabela eh preenchida ate
// alpha (sem medir) e entao mede-se a insercao de um lote pequeno de chaves novas
#define L_ENCAD 16       // encadeamento: m = 2^16 = 65536
#define M_ABERTO 65537   // enderecamento aberto: 2^16 + 1 (primo)
#define PROB_LINEAR 1    // 1: sondagem linear; 0: hashing duplo

#define REPETICOES 20
#define LOTE (M_ABERTO / 200) // ~0.5% de m: alpha praticamente nao muda durante o lote

int main() {
    rng_semear((uint64_t) time(NULL));

    double alphas[] = {0.1, 0.5, 0.9, 0.99};
    int num_alphas = sizeof(alphas) / sizeof(alphas[0]);
    int m_encad = 1 << L_ENCAD;

    printf("m = %d (encadeamento), %d (aberto, %s); lote de %d insercoes, media de %d tabelas\n",
           m_encad, M_ABERTO, PROB_LINEAR ? "linear" : "duplo", LOTE, REPETICOES);
    printf("sondagens: posicoes examinadas (o encadeamento insere na cabeca da lista, sem sondar)\n\n");
    printf("%-6s | %-12s | %-12s | %s\n", "alpha", "encad_ns", "aberto_ns", "aberto_sondagens");

    for (int t = 0; t < num_alphas; t++) {
        double a = alphas[t];
        int n_encad = (int) (a * m_encad); // alpha = n/m entao n = alpha * m
        int n_aberto = (int) (a * M_ABERTO);
        int n_max = n_encad > n_aberto ? n_encad : n_aberto;
        medida encad = {0, 0}, abert = {0, 0};

        for (int r = 0; r < REPETICOES; r++) {
            // preenchemos ate atingir o fator de carga
            int *A = gerar_chaves(n_max + LOTE, 0);
            const int *lote = A + n_max;

            hash_encadeamento *He = encad_criar(L_ENCAD);
            for (int i = 0; i < n_encad; i++) {
                encad_inserir(He, A[i]);
            }

            // comecar a medicao com o fator de carga estabelecido
            // NOTE: medimos o tempo de varias insercoes (lote) e depois calculamos a media
            // da insercao no lote de chaves, depois a media em relacao as repeticoes feitas
            double inicio = agora_ns();
            for (int i = 0; i < LOTE; i++) {
                encad_inserir(He, lote[i]);
            }
            encad.ns += (agora_ns() - inicio) / LOTE / REPETICOES;
            encad_liberar(He);

            hash_aberto *Ha = aberto_criar(M_ABERTO, PROB_LINEAR);
            for (int i = 0; i < n_aberto; i++) {
                aberto_inserir(Ha, A[i]);
            }
            long ok = 0;
            sondagens = 0;
            inicio = agora_ns();
            for (int i = 0; i < LOTE; i++) {
                ok += aberto_inserir(Ha, lote[i]) != -1;
            }
            abert.ns += (agora_ns() - inicio) / LOTE / REPETICOES;
            abert.sondagens += (double) sondagens / LOTE / REPETICOES;
            sink = ok;
            aberto_liberar(Ha);

            free(A);
        }

        printf("%-6.2f | %-12.1f | %-12.1f | %.2f\n", a,
               encad.ns, abert.ns, abert.sondagens);
    }
    return 0;
}
