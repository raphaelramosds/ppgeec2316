#include "test_hash_utils.h"
#include "hash.h"
#include "hash_aberto.h"
#include "hash_encadeamento.h"

// Ciclos de remove + insere com alpha = 0.5 constante, medindo a busca sem sucesso: no
// enderecamento aberto os DELETED se acumulam e passam a ser visitados pelas sondagens
#define L_ENCAD 16       // encadeamento: m = 2^16 = 65536
#define M_ABERTO 65537   // enderecamento aberto: 2^16 + 1 (primo)
#define PROB_LINEAR 1    // 1: sondagem linear; 0: hashing duplo

#define NUM_FALHAS 1000  // buscas sem sucesso em cada ponto de medicao
#define MAX_CICLOS (8 * M_ABERTO)

int main() {
    rng_semear((uint64_t) time(NULL));

    int n = (1 << L_ENCAD) / 2; // alpha = 0.5 nas duas tabelas

    // A[0..n) sao as chaves iniciais e A[n..n + MAX_CICLOS) as inseridas a cada ciclo
    int *A = gerar_chaves(n + MAX_CICLOS, 0);
    int *B = gerar_chaves(NUM_FALHAS, 1);
    int *presentes = (int*) malloc(n * sizeof(int));
    if (presentes == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }

    hash_encadeamento *He = encad_criar(L_ENCAD);
    hash_aberto *Ha = aberto_criar(M_ABERTO, PROB_LINEAR);
    for (int i = 0; i < n; i++) {
        presentes[i] = A[i];
        encad_inserir(He, A[i]);
        aberto_inserir(Ha, A[i]);
    }

    printf("ciclos de remove + insere com alpha = 0.5 constante (n = %d)\n", n);
    printf("m = %d (encadeamento), %d (aberto, %s)\n", 1 << L_ENCAD, M_ABERTO,
           PROB_LINEAR ? "linear" : "duplo");
    printf("cada ciclo remove uma chave presente ao acaso e insere uma chave nova\n");
    printf("busca: %d buscas sem sucesso apos o numero de ciclos indicado\n\n", NUM_FALHAS);
    printf("%-10s | %-10s %-10s | %-16s %-12s | %-16s %-12s\n", "ciclos",
           "%NIL", "%DELETED", "encad_sondagens", "encad_ns", "aberto_sondagens", "aberto_ns");

    // pontos de medicao: 0, m/8, m/4, m/2, m, 2m, 4m, 8m ciclos
    int pontos[] = {0, M_ABERTO / 8, M_ABERTO / 4, M_ABERTO / 2, M_ABERTO,
                    2 * M_ABERTO, 4 * M_ABERTO, 8 * M_ABERTO};
    int num_pontos = sizeof(pontos) / sizeof(pontos[0]);

    int ciclo = 0;
    for (int p = 0; p < num_pontos; p++) {
        for (; ciclo < pontos[p]; ciclo++) {
            int i = (int) (rng_proximo() % (uint64_t) n);
            int nova = A[n + ciclo];
            encad_remover(He, presentes[i]);
            aberto_remover(Ha, presentes[i]);
            encad_inserir(He, nova);
            if (aberto_inserir(Ha, nova) == -1) {
                fprintf(stderr, "Tabela cheia no ciclo %d.\n", ciclo);
                exit(1);
            }
            presentes[i] = nova;
        }

        int nil = 0, deleted = 0;
        for (int j = 0; j < Ha->m; j++) {
            nil += Ha->T[j] == NIL;
            deleted += Ha->T[j] == DELETED;
        }

        medida e = buscar_encad(He, B, NUM_FALHAS), a = buscar_aberto(Ha, B, NUM_FALHAS);
        printf("%-10d | %-10.2f %-10.2f | %-16.2f %-12.1f | %-16.2f %-12.1f\n", pontos[p],
               100.0 * nil / Ha->m, 100.0 * deleted / Ha->m, e.sondagens, e.ns, a.sondagens, a.ns);
    }

    encad_liberar(He);
    aberto_liberar(Ha);
    free(A);
    free(B);
    free(presentes);
    return 0;
}
