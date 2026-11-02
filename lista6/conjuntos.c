#include "conjuntos.h"

void make_set(node *x) {
    x->p = x;
    x->rank = 0;
}

node *find_set(node *x) {
    if (x != x->p) {
        // se nao eh a raiz (representante), continue procurando recursivamente
        x->p = find_set(x->p);
    }
    // retorne a raiz
    return x->p;
}

static void link_set(node *x, node *y) {
    // a raiz de menor rank passa a apontar para a de maior rank
    if (x->rank > y->rank) {
        y->p = x;
    } else {
        x->p = y;
        if (x->rank == y->rank) {
            y->rank++;
        }
    }
}

void union_set(node *x, node *y) {
    link_set(find_set(x), find_set(y));
}
