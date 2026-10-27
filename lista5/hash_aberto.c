#include "hash_aberto.h"
#include "hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

hash_aberto *aberto_criar(int m, int prob_linear) {
    hash_aberto *H = (hash_aberto*) malloc(sizeof(hash_aberto));
    H->m = m;
    H->n = 0;
    H->prob_linear = prob_linear;

    H->T = (int*) malloc(H->m * sizeof(int));
    if (H->T == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    for (int j = 0; j < H->m; j++) {
        H->T[j] = NIL;
    }
    return H;
}

void aberto_liberar(hash_aberto *H) {
    free(H->T);
    free(H);
}

// posicao inicial da sequencia de sondagem: metodo da divisao
static inline uint32_t h1(const hash_aberto *H, int k) {
    return (uint32_t) k % H->m;
}

// passo da sequencia de sondagem: 1 <= h2(k) <= m - 1, logo primo com m (que eh primo)
static inline uint32_t h2(const hash_aberto *H, int k) {
    return 1 + (uint32_t) k % (H->m - 1);
}

// proxima posicao da sequencia de sondagem
static inline uint32_t proxima_sondagem(const hash_aberto *H, uint32_t q, uint32_t passo) {
    q += passo;
    return q >= (uint32_t) H->m 
        ? q - H->m // alcancou final da tabela, volte para o inicio
        : q;
}

int aberto_inserir(hash_aberto *H, int k) {
    // sondagem linear: h2(k) = 1
    uint32_t q = h1(H, k), passo = H->prob_linear ? 1 : h2(H, k);

    for (int i = 0; i < H->m; i++) {
        sondagens++;
        // a primeira posicao livre (vazia ou removida) na sequencia de sondagem recebe k
        if (H->T[q] == NIL || H->T[q] == DELETED) {
            H->T[q] = k;
            H->n++;
            return (int) q;
        }
        q = proxima_sondagem(H, q, passo);
    }
    return -1; // estouro da tabela hash
}

int aberto_buscar(const hash_aberto *H, int k) {
    // sondagem linear: h2(k) = 1
    uint32_t q = h1(H, k), passo = H->prob_linear ? 1 : h2(H, k);

    // percorre a mesma sequencia de sondagem usada quando k foi inserida
    for (int i = 0; i < H->m; i++) {
        sondagens++;
        // NOTE: ao encontrar NIL, k nao pode estar mais adiante, pois teria sido inserida ali
        if (H->T[q] == NIL) {
            return -1;
        }
        if (H->T[q] == k) {
            return (int) q;
        }
        q = proxima_sondagem(H, q, passo);
    }
    return -1;
}

int aberto_remover(hash_aberto *H, int k) {
    // NOTE: aberto_buscar ja percorre a sequencia de sondagem conforme H->prob_linear
    int q = aberto_buscar(H, k);
    if (q != -1) {
        // NOTE: nao podemos usar NIL aqui, senao chaves inseridas depois de k na mesma sequencia
        // de sondagem ficariam inacessiveis para a busca
        H->T[q] = DELETED;
        H->n--;
    }
    return q;
}
