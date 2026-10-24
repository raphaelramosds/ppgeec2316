#include "rubro_negra.h"
#include <stdio.h>
#include <stdlib.h>

arvore_rn *rn_criar(void) {
    arvore_rn *T = (arvore_rn*) malloc(sizeof(arvore_rn));
    T->nil = (no_rn*) malloc(sizeof(no_rn));
    T->nil->cor = PRETO;
    T->nil->left = T->nil->right = T->nil->p = T->nil;
    T->root = T->nil;
    return T;
}

static void liberar_subarvore(arvore_rn *T, no_rn *x) {
    if (x == T->nil) {
        return;
    }
    liberar_subarvore(T, x->left);
    liberar_subarvore(T, x->right);
    free(x);
}

void rn_liberar(arvore_rn *T) {
    // NOTE: a recursao eh segura pois a altura eh no maximo 2 lg(n + 1)
    liberar_subarvore(T, T->root);
    free(T->nil);
    free(T);
}

// LEFT-ROTATE: x desce para a esquerda e seu filho direito y ocupa seu lugar
//
//      x                y
//     / \              / \    .
//    a   y     =>     x   c
//       / \          / \      .
//      b   c        a   b
static void rotacao_esquerda(arvore_rn *T, no_rn *x) {
    no_rn *y = x->right;

    // subarvore esquerda de y vira subarvore direita de x
    x->right = y->left;
    if (y->left != T->nil) {
        y->left->p = x;
    }

    // o pai de x passa a ser o pai de y
    y->p = x->p;
    if (x->p == T->nil) {
        T->root = y;
    } else if (x == x->p->left) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }

    // x vira filho esquerdo de y
    y->left = x;
    x->p = y;
}

// RIGHT-ROTATE: simetrico ao LEFT-ROTATE (troca left <-> right)
static void rotacao_direita(arvore_rn *T, no_rn *x) {
    no_rn *y = x->left;

    x->left = y->right;
    if (y->right != T->nil) {
        y->right->p = x;
    }

    y->p = x->p;
    if (x->p == T->nil) {
        T->root = y;
    } else if (x == x->p->right) {
        x->p->right = y;
    } else {
        x->p->left = y;
    }

    y->right = x;
    x->p = y;
}

// RB-INSERT-FIXUP: restaura as propriedades rubro-negras apos inserir o no vermelho z
// NOTE: a unica propriedade que pode ser violada eh a 4 (no vermelho com filho vermelho)
// ou a 2 (raiz vermelha)
static void rn_inserir_fixup(arvore_rn *T, no_rn *z) {
    while (z->p->cor == VERMELHO) {
        if (z->p == z->p->p->left) {
            // o pai de z eh filho esquerdo
            no_rn *y = z->p->p->right; // tio de z

            if (y->cor == VERMELHO) {
                // caso 1: tio vermelho -> recolore e sobe o problema dois niveis
                z->p->cor = PRETO;
                y->cor = PRETO;
                z->p->p->cor = VERMELHO;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    // caso 2: tio preto e z eh filho direito -> rotaciona e cai no caso 3
                    z = z->p;
                    rotacao_esquerda(T, z);
                }
                // caso 3: tio preto e z eh filho esquerdo -> recolore e rotaciona o avo
                z->p->cor = PRETO;
                z->p->p->cor = VERMELHO;
                rotacao_direita(T, z->p->p);
            }
        } else {
            // simetrico: o pai de z eh filho direito
            no_rn *y = z->p->p->left;

            if (y->cor == VERMELHO) {
                z->p->cor = PRETO;
                y->cor = PRETO;
                z->p->p->cor = VERMELHO;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    z = z->p;
                    rotacao_direita(T, z);
                }
                z->p->cor = PRETO;
                z->p->p->cor = VERMELHO;
                rotacao_esquerda(T, z->p->p);
            }
        }
    }
    T->root->cor = PRETO;
}

void rn_inserir(arvore_rn *T, int k) {
    no_rn *z = (no_rn*) malloc(sizeof(no_rn));
    if (z == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(1);
    }
    z->chave = k;

    // desce a arvore como em TREE-INSERT, guardando em y o pai do novo no
    no_rn *x = T->root;
    no_rn *y = T->nil;
    while (x != T->nil) {
        y = x;
        if (z->chave < x->chave) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->p = y;
    if (y == T->nil) {
        T->root = z; // arvore estava vazia
    } else if (z->chave < y->chave) {
        y->left = z;
    } else {
        y->right = z;
    }

    // o novo no entra vermelho (nao altera a altura negra) com ambos os filhos em T.nil
    z->left = T->nil;
    z->right = T->nil;
    z->cor = VERMELHO;

    rn_inserir_fixup(T, z);
}

no_rn *rn_buscar(const arvore_rn *T, int k) {
    no_rn *x = T->root;
    while (x != T->nil && k != x->chave) {
        if (k < x->chave) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return x;
}
