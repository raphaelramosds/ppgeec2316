#ifndef CACHE_RANDOM_H
#define CACHE_RANDOM_H

#include "cache.h"

void random_evict(Cache *c, Block *b);

int random_get_block(Cache *c, int block_id);

#endif
