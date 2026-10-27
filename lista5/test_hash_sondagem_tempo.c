#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"

// Sondagem linear x hashing duplo: tempo de busca com uma tabela maior que a cache
// NOTE: com m = 2^23 + 9, a tabela ocupa ~32 MB (> L3). A sondagem linear examina posicoes
// vizinhas (mesma linha de cache), enquanto o hashing duplo salta para posicoes distantes
// a cada sondagem. Logo o custo por sondagem (ns/sond) deve ser menor na sondagem linear
#define M_ABERTO 8388617  // 2^23 + 9 (primo)

#define REPETICOES 3        // tabelas construidas (com chaves novas) para cada alpha
#define NUM_SUCESSOS 1000000 // buscas com sucesso por tabela (amostra das chaves presentes)
#define NUM_FALHAS 100000    // buscas sem sucesso por tabela (caras na sondagem linear com alpha ~ 1)

int main() {
    rng_semear((uint64_t) time(NULL));

    double alphas[] = {0.1, 0.25, 0.5, 0.75, 0.9, 0.95, 0.99};
    int num_alphas = sizeof(alphas) / sizeof(alphas[0]);

    // [estrategia][0: com sucesso, 1: sem sucesso], estrategia 0: linear, 1: duplo
    medida res[num_alphas][2][2];

    for (int t = 0; t < num_alphas; t++) {
        int n = (int) (alphas[t] * M_ABERTO);
        int q_ok = n < NUM_SUCESSOS ? n : NUM_SUCESSOS;

        for (int e = 0; e < 2; e++) {
            for (int s = 0; s < 2; s++) {
                res[t][e][s] = (medida) {0, 0};
            }
        }

        for (int r = 0; r < REPETICOES; r++) {
            // as mesmas chaves (pares) nas duas tabelas e as mesmas chaves ausentes (impares)
            int *A = gerar_chaves(n, 0);
            int *B = gerar_chaves(NUM_FALHAS, 1);

            // amostra das presentes para a busca com sucesso
            // NOTE: nao pode ser A[0..q_ok), pois as primeiras chaves inseridas encontraram a
            // tabela quase vazia e tem sequencias de sondagem curtas
            int *S = (int*) malloc(n * sizeof(int));
            if (S == NULL) {
                fprintf(stderr, "Erro de alocacao de memoria.\n");
                exit(1);
            }
            for (int i = 0; i < n; i++) {
                S[i] = A[i];
            }
            embaralhar(S, n);

            for (int e = 0; e < 2; e++) {
                hash_aberto *H = aberto_criar(M_ABERTO, e == 0); // e == 0: sondagem linear
                for (int i = 0; i < n; i++) {
                    aberto_inserir(H, A[i]);
                }
                medida ok = buscar_aberto(H, S, q_ok), falha = buscar_aberto(H, B, NUM_FALHAS);
                res[t][e][0].sondagens += ok.sondagens / REPETICOES;
                res[t][e][0].ns += ok.ns / REPETICOES;
                res[t][e][1].sondagens += falha.sondagens / REPETICOES;
                res[t][e][1].ns += falha.ns / REPETICOES;
                aberto_liberar(H);
            }

            free(A);
            free(B);
            free(S);
        }
    }

    printf("m = %d (%.1f MB); media de %d tabelas por alpha\n",
           M_ABERTO, M_ABERTO * sizeof(int) / 1048576.0, REPETICOES);
    printf("sond: posicoes examinadas por busca; ns: tempo por busca; ns/sond: tempo por sondagem\n");

    const char *titulos[] = {"busca com sucesso", "busca sem sucesso"};
    for (int s = 0; s < 2; s++) {
        printf("\n%s\n\n", titulos[s]);
        printf("%-6s | %-10s %-10s %-10s | %-10s %-10s %-10s\n", "alpha",
               "lin_sond", "lin_ns", "lin_ns/so", "dup_sond", "dup_ns", "dup_ns/so");
        for (int t = 0; t < num_alphas; t++) {
            printf("%-6.2f |", alphas[t]);
            for (int e = 0; e < 2; e++) {
                medida x = res[t][e][s];
                printf(" %-10.2f %-10.1f %-10.2f%s", x.sondagens, x.ns, x.ns / x.sondagens,
                       e == 0 ? " |" : "\n");
            }
        }
    }
    return 0;
}
