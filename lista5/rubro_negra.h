#ifndef RUBRO_NEGRA_H
#define RUBRO_NEGRA_H

#define VERMELHO 0
#define PRETO 1

// Arvore rubro-negra (CLRS capitulo 13)
typedef struct no_rn {
    int chave;
    int cor;
    struct no_rn *left;
    struct no_rn *right;
    struct no_rn *p;
} no_rn;

// NOTE: assim como no livro, todas as folhas e o pai da raiz apontam para a sentinela T.nil
typedef struct {
    no_rn *root;
    no_rn *nil; // ao inves de criar varios nos folhas como NILL, apontamos para este sentinela
} arvore_rn;

arvore_rn *rn_criar(void);
void rn_liberar(arvore_rn *T);

// RB-INSERT: insere a chave k em O(lg n)
void rn_inserir(arvore_rn *T, int k);

// TREE-SEARCH (versao iterativa): O(h) = O(lg n). Retorna o no ou T->nil
no_rn *rn_buscar(const arvore_rn *T, int k);

#endif
