#ifndef HASH_ABERTO_H
#define HASH_ABERTO_H

// valores especiais das posicoes (as chaves devem ser nao negativas)
#define NIL (-1)
#define DELETED (-2)

// Tabela hash com enderecamento aberto e hashing duplo (CLRS secao 11.4):
//     h(k, i) = (h1(k) + i * h2(k)) mod m
// com h1 e h2 pelo metodo da divisao (CLRS secao 11.3):
//     h1(k) = k mod m
//     h2(k) = 1 + (k mod (m - 1))
// NOTE: com m primo e 1 <= h2(k) < m, h2(k) eh primo com m e a sequencia de sondagem
// <h(k,0), ..., h(k,m-1)> eh uma permutacao de <0, ..., m-1>
// Com prob_linear = 1, usa sondagem linear, caso especial do hashing duplo com h2(k) = 1:
//     h(k, i) = (h1(k) + i) mod m
typedef struct {
    int *T;
    int m; // numero de posicoes, deve ser primo (de preferencia longe de uma potencia de 2)
    int n; // numero de chaves armazenadas
    int prob_linear; // 1: sondagem linear; 0: hashing duplo
} hash_aberto;

hash_aberto *aberto_criar(int m, int prob_linear);
void aberto_liberar(hash_aberto *H);

// HASH-INSERT: retorna a posicao onde k foi armazenada ou -1 se a tabela estiver cheia
// NOTE: posicoes DELETED sao tratadas como vazias. Assume que k ainda nao esta na tabela
int aberto_inserir(hash_aberto *H, int k);

// HASH-SEARCH: retorna a posicao de k ou -1 (NIL) se k nao estiver na tabela
// NOTE: posicoes DELETED sao puladas, a busca so para em NIL (ou apos m sondagens)
int aberto_buscar(const hash_aberto *H, int k);

// Remove k marcando sua posicao como DELETED. Retorna a posicao ou -1 se k nao estiver na tabela
int aberto_remover(hash_aberto *H, int k);

#endif
