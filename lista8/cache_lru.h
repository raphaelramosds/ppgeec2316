#ifndef CACHE_LRU_H
#define CACHE_LRU_H

#include "cache.h"

// Politica LRU: remove do cache o bloco usado menos recentemente (menor ts)
void lru_evict(Cache *c);

// Busca o bloco block_id no cache
// Funcionamento:
//  - A cada acesso o relogio do cache (clock) avanca
//  - Se encontrado (hit): atualiza ts desse bloco com o clock atual e retorna 1
//  - Se nao encontrado (miss): incrementa misses e retorna 0. E se houver espaço livre, insere o bloco com ts = clock, caso contrario, chama lru_evict(c) antes de inserir
int lru_get_block(Cache *c, int block_id);

#endif
