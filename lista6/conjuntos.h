#ifndef CONJUNTOS_H
#define CONJUNTOS_H

typedef struct node {
    // pai do no
    struct node *p;
    // rank (posto)
    int rank;
} node;

// cria um conjunto cujo unico elemento eh x
void make_set(node *x);

// retorna o representante do conjunto que contem x
node *find_set(node *x);

// une os conjuntos que contem x e y
void union_set(node *x, node *y);

#endif
