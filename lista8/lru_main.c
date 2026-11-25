#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache.h"

static void gerar_requisicoes(int *req, int n, int k) {
    int idx = 0;
    while (idx < n) {
        int distinct_count = 0;
        int seen[11] = {0}; // contagem dos blocos sorteados (1 a 10)

        while (idx < n) {
            // sorteia um bloco candidato entre 1 e 10
            // NOTE: o bloco 0 nunca eh sorteado
            int candidate = (rand() % 10) + 1;
            if (!seen[candidate]) {
                if (distinct_count == k) {
                    // o numero de blocos distintos em sequencia nao pode ser maior do que a capacidade do cache.
                    // inicia nova epoca no proximo laco externo
                    break;
                }
                // Conta como um bloco distinto
                seen[candidate] = 1;
                distinct_count++;
            }
            req[idx++] = candidate;
        }
    }
}

static void seer_evict(int *opt_cache, int opt_size, const int *req, int n, int pos) {
    // opt_cache: estado atual do cache
    // opt_size: qtde de blocos inseridos na cache
    // req: lista de requisicoes aos blocos
    // n: qtde de requisicoes
    // pos: posicao do bloco na lista de requisicoes que se quer colocar na cache
    int victim_idx = 0;
    int farthest_dist = -1;
    
    // para cada bloco (opt_cache[i]) que ja esta na cache
    // consulte o vetor de requisicoes (req) e descubra quando opt_cache[i] vai ser solicitado novamente (idx_next_request)
    // o bloco (victim_idx) que sera utilizado mais tarde (farthest_dist) sera removido
    for (int i = 0; i < opt_size; i++) {
        int idx_next_request = n + 1;
        for (int j = pos + 1; j < n; j++) {
            if (req[j] == opt_cache[i]) {
                idx_next_request = j;
                break;
            }
        }

        if (idx_next_request > farthest_dist) {
            farthest_dist = idx_next_request;
            victim_idx = i;
        }
    }

    // desloque o bloco que vai ser removido para a ultima linha de cache
    for (int i = victim_idx; i < opt_size - 1; i++) {
        opt_cache[i] = opt_cache[i + 1];
    }

    // coloque o novo bloco na posicao dele
    opt_cache[opt_size - 1] = req[pos];
}

static int seer_simular(const int *req, int n, int k) {
    // implementacao de cache interna para o oraculo: capacidade k e inicialmente vazia
    int *opt_cache = (int *)malloc((size_t)k * sizeof(int));
    int opt_size = 0;
    int misses = 0;

    // simulacao com a mesma lista de sequencias lida pelo LRU
    for (int t = 0; t < n; t++) {
        int b = req[t];
        int hit = 0; // flag: 0 (nao achou o bloco no cache) ou 1 (achou)

        // busca o bloco no cache
        for (int i = 0; i < opt_size; i++) {
            if (opt_cache[i] == b) {
                // encontrado
                hit = 1;
                break;
            }
        }

        // tratar o cache miss
        if (!hit) {
            misses++;
            if (opt_size < k) {
                // tem espaco para mais um bloco
                opt_cache[opt_size++] = b;
            } else {
                // sem espaco: aplique o algoritmo offline de remocao
                seer_evict(opt_cache, opt_size, req, n, t);
                
            }
        }
    }

    free(opt_cache);
    return misses;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s N [K]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int k = (argc >= 3) ? atoi(argv[2]) : 3; // padrao k = 3

    if (n <= 0 || k <= 0) {
        fprintf(stderr, "N e K devem ser inteiros positivos.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    int *req = (int *)malloc((size_t)n * sizeof(int));
    gerar_requisicoes(req, n, k);

    // simulacao do LRU
    Cache *c = init_cache(k);
    if (c == NULL) {
        fprintf(stderr, "Falha ao alocar o cache.\n");
        free(req);
        return 1;
    }
    for (int t = 0; t < n; t++) {
        get_block(c, req[t]);
    }
    int misses_lru = c->misses;

    // simulacao do oraculo (seer)
    int misses_seer = seer_simular(req, n, k);

    // calculo da taxa de competividade
    double ratio = (misses_seer > 0) ? ((double)misses_lru / (double)misses_seer) : 0.0;

    printf("N = %d | K = %d\n", n, k);
    printf("cache_misses_lru       = %d\n", misses_lru);
    printf("cache_misses_seer_evict = %d\n", misses_seer);
    printf("razao (LRU / Seer)     = %.4f (limite teorico <= %d)\n", ratio, k);

    free_cache(c);
    free(req);
    return 0;
}