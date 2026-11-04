#ifndef TEMPO_H
#define TEMPO_H

#include <time.h>

// static para que cada programa C tenha sua propria copia desta funcao
static inline double tempo_atual(void) {
    struct timespec t;
    // retorna o tempo de parede (wall time) com precisao em nanosegundos
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

#endif