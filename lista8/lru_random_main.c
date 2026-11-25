#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cache_lru.h"
#include "cache_random.h"

// tamanho padrao de um bloco (linha de cache) em bytes
#define BLOCK_SIZE_PADRAO 64

static int endereco_para_bloco(unsigned long endereco, unsigned long block_size) {
    // mapeamento direto: endereco -> indice do bloco que o contem
    return (int)(endereco / block_size);
}

static int *ler_requisicoes(const char *caminho, unsigned long block_size, int *n) {
    // n: qtde de requisicoes
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        perror(caminho);
        return NULL;
    }

    char linha[64];
    char *fim;

    // 1a passada: conta as linhas nao vazias (cada uma contem um endereco)
    int total = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        strtoul(linha, &fim, 16);
        if (fim == linha) continue; // linha vazia ou invalida
        total++;
    }

    // aloca exatamente a quantidade de enderecos encontrada
    // NOTE: com total == 0, malloc(0) pode retornar NULL; aloca ao menos 1
    int *req = (int *)malloc((size_t)(total > 0 ? total : 1) * sizeof(int));
    if (req == NULL) {
        fclose(f);
        return NULL;
    }

    // 2a passada: volta ao inicio do arquivo e converte cada endereco em bloco
    rewind(f);
    int qtd = 0;
    while (qtd < total && fgets(linha, sizeof(linha), f) != NULL) {
        unsigned long endereco = strtoul(linha, &fim, 16); // converte string hexadecimal em long
        if (fim == linha) continue;
        req[qtd++] = endereco_para_bloco(endereco, block_size);
    }

    fclose(f);
    *n = qtd;
    return req;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s TRACE [K] [SEED] [BLOCK_SIZE]\n", argv[0]);
        fprintf(stderr, "  TRACE       arquivo .data.trace (um endereco hex por linha)\n");
        fprintf(stderr, "  K           capacidade do cache em blocos (padrao 3)\n");
        fprintf(stderr, "  SEED        semente da marcacao aleatoria (padrao time(NULL))\n");
        fprintf(stderr, "  BLOCK_SIZE  tamanho do bloco em bytes (padrao %d)\n", BLOCK_SIZE_PADRAO);
        return 1;
    }

    const char *caminho = argv[1];
    int k = (argc >= 3) ? atoi(argv[2]) : 3; // padrao k = 3
    unsigned int seed = (argc >= 4) ? (unsigned int)strtoul(argv[3], NULL, 10)
                                    : (unsigned int)time(NULL);
    unsigned long block_size = (argc >= 5) ? strtoul(argv[4], NULL, 10) : BLOCK_SIZE_PADRAO;

    if (k <= 0 || block_size == 0) {
        fprintf(stderr, "K e BLOCK_SIZE devem ser inteiros positivos.\n");
        return 1;
    }

    // lista de requisicoes de blocos extraida do traco de memoria
    int n = 0;
    int *req = ler_requisicoes(caminho, block_size, &n);
    if (req == NULL) {
        fprintf(stderr, "Falha ao ler as requisicoes de %s.\n", caminho);
        return 1;
    }
    if (n == 0) {
        fprintf(stderr, "Nenhum endereco encontrado em %s.\n", caminho);
        free(req);
        return 1;
    }

    // duas caches com o mesmo k
    Cache *c_lru = init_cache(k);
    Cache *c_rand = init_cache(k);
    if (c_lru == NULL || c_rand == NULL) {
        fprintf(stderr, "Falha ao alocar o cache.\n");
        free_cache(c_lru);
        free_cache(c_rand);
        free(req);
        return 1;
    }

    // simulacao do LRU (lru_evict de cache_lru.c)
    for (int t = 0; t < n; t++) {
        lru_get_block(c_lru, req[t]);
    }

    // simulacao da marcacao aleatoria (random_evict)
    // mesma seed: execucoes com a mesma seed sao reprodutiveis
    srand(seed);
    for (int t = 0; t < n; t++) {
        random_get_block(c_rand, req[t]);
    }

    int misses_lru = c_lru->misses;
    int misses_rand = c_rand->misses;
    double ratio = (misses_lru > 0) ? ((double)misses_rand / (double)misses_lru) : 0.0;

    printf("TRACE = %s | N = %d | K = %d | SEED = %u | BLOCK_SIZE = %lu\n",
           caminho, n, k, seed, block_size);
    printf("cache_misses_lru    = %d\n", misses_lru);
    printf("cache_misses_random = %d\n", misses_rand);
    printf("razao (Random / LRU) = %.4f\n", ratio);

    free_cache(c_lru);
    free_cache(c_rand);
    free(req);
    return 0;
}
