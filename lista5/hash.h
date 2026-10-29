#ifndef HASH_H
#define HASH_H

#include <stdint.h>

// contador global de sondagens usado nos experimentos (definido em hash.c):
// - enderecamento aberto: posicoes da tabela examinadas (inclusive a vazia que encerra a busca)
// - encadeamento: nos da lista examinados
extern long sondagens;

// Metodo multiply-shift (CLRS secao 11.3, equacao 11.2), com w = 32 e m = 2^l:
//     h_a(k) = (k * a mod 2^w) >> (w - l)
// isto eh, os l bits mais significativos da metade inferior do produto k * a
// NOTE: a multiplicacao de uint32_t ja eh feita modulo 2^32. Requer 1 <= l <= 32
static inline uint32_t multiply_shift(uint32_t k, uint32_t a, int l) {
    return (k * a) >> (32 - l);
}

#endif
