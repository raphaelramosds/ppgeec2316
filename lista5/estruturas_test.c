#include "array_ordenado.h"
#include "lista_encadeada.h"
#include "rubro_negra.h"
#include <stdio.h>
#include <stdlib.h>

static int falhas = 0;

#define VERIFICA(cond, msg) do { \
    if (!(cond)) { printf("FALHOU: %s\n", msg); falhas++; } \
} while (0)

// Verifica as propriedades rubro-negras e a ordem da arvore de busca na subarvore de x.
// Retorna a altura negra da subarvore ou -1 se alguma propriedade for violada
static int altura_negra(const arvore_rn *T, const no_rn *x) {
    if (x == T->nil) {
        return 1; // propriedade 3: toda folha (T.nil) eh preta
    }
    // propriedade 4: no vermelho tem ambos os filhos pretos
    if (x->cor == VERMELHO && (x->left->cor == VERMELHO || x->right->cor == VERMELHO)) {
        return -1;
    }
    // propriedade de arvore de busca binaria
    if ((x->left != T->nil && x->left->chave > x->chave) ||
        (x->right != T->nil && x->right->chave < x->chave)) {
        return -1;
    }
    int he = altura_negra(T, x->left);
    int hd = altura_negra(T, x->right);
    // propriedade 5: mesmo numero de nos pretos em todos os caminhos ate as folhas
    if (he == -1 || hd == -1 || he != hd) {
        return -1;
    }
    return he + (x->cor == PRETO);
}

// Usa chaves pares para que qualquer chave impar seja garantidamente ausente
static void testar_estruturas(int n) {
    array_ordenado *a = array_criar();
    lista_encadeada *L = lista_criar();
    arvore_rn *T = rn_criar();

    for (int k = 0; k < n; k++) {
        int x = 2 * (rand() % (5 * n + 1));
        array_inserir(a, x);
        lista_inserir(L, x);
        rn_inserir(T, x);
    }

    VERIFICA(a->tamanho == n, "array tem n elementos");
    for (int k = 1; k < a->tamanho; k++) {
        if (a->chaves[k - 1] > a->chaves[k]) {
            VERIFICA(0, "array esta ordenado");
            break;
        }
    }

    VERIFICA(T->root->cor == PRETO, "raiz da arvore eh preta");
    VERIFICA(altura_negra(T, T->root) != -1, "propriedades rubro-negras e de BST");

    // toda chave inserida deve ser encontrada nas tres estruturas
    for (int k = 0; k < a->tamanho; k++) {
        int x = a->chaves[k];
        if (array_buscar(a, x) == -1 || lista_buscar(L, x) == NULL || rn_buscar(T, x) == T->nil) {
            VERIFICA(0, "busca por chave presente");
            break;
        }
    }
    // chaves impares nunca foram inseridas
    for (int x = 1; x < 10 * n; x += 2) {
        if (array_buscar(a, x) != -1 || lista_buscar(L, x) != NULL || rn_buscar(T, x) != T->nil) {
            VERIFICA(0, "busca por chave ausente");
            break;
        }
    }

    array_liberar(a);
    lista_liberar(L);
    rn_liberar(T);
}

int main() {
    srand(42);

    int tamanhos[] = {0, 1, 2, 3, 10, 100, 1000, 5000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    for (int t = 0; t < num_tamanhos; t++) {
        testar_estruturas(tamanhos[t]);
    }

    // insercoes em ordem crescente: pior caso de uma BST nao balanceada
    arvore_rn *T = rn_criar();
    for (int k = 0; k < 10000; k++) {
        rn_inserir(T, k);
    }
    VERIFICA(altura_negra(T, T->root) != -1, "arvore balanceada apos insercao ordenada");
    rn_liberar(T);

    if (falhas == 0) {
        printf("Todos os testes passaram.\n");
    }
    return falhas != 0;
}
