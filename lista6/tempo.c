// clock_gettime/CLOCK_MONOTONIC nao fazem parte do C99 puro
#define _POSIX_C_SOURCE 199309L

#include "tempo.h"
#include <time.h>

double tempo_atual(void) {
    // relogio monotonico: nao sofre ajustes do relogio do sistema
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}
