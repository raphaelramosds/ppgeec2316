#include "selection.h"
#include "utils.h"

int partition(int A[], int p, int r) {
    // pivo
    int x = A[r];

    // representa o ultimo indice da particao esquerda
    int i = p - 1;

    for (int j = p; j < r; j++) {
        // troca se achar um elemento menor que o pivo
        // para posiciona-lo na particao esquerda (a particao com os menores elementos)
        if (A[j] <= x) {
            i++;
            swap(&A[i], &A[j]);
        }
    }

    // coloca pivo na posicao correta
    // NOTE: "i + 1" se encontra apos o ultimo indice "i" da particao esquerda 
    swap(&A[i + 1], &A[r]);

    return i + 1;
}

int randomized_partition(int A[], int p, int r) {
    // escolhe uma posicao entre p e r aleatoriamente
    int i = p + rand() % (r - p + 1);

    // coloca o pivo no final da particao
    // NOTE: se ele for o maior/menor elemento da particao, a divisao fica desbalanceada
    // atingindo, o pior caso do algoritmo
    swap(&A[i], &A[r]);

    // posiciona elementos menores que o pivo a esquerda e maiores a direita
    return partition(A, p, r);
}

// seleciona o i-esimo menor valor em A[p:r]
int randomized_select(int A[], int p, int r, int i) {
    
    int q, k;

    if (p == r) {
        // particao tem apenas um elemento
        return A[p];
    }

    // escolha o pivo aleatoriamente
    q = randomized_partition(A, p, r);

    // rank de A[q]: k-esimo menor valor da particao esquerda
    // exemplo: [2,5,8,9,10], rank(5) = 3 elementos menores ou iguais a 5 
    k = q - p + 1;

    if (i == k) {
        // calculamos um rank da posicao desejada (o "i") entao retorne
        return A[q];
    } else if (i < k) {
        // calculamos um rank maior do que o desejado, entao procure APENAS na particao esquerda
        return randomized_select(A, p, q - 1, i);
    } else {
        // calculamos um rank menor do que o desejado, entao procure APENAS na particao direita
        // NOTE: achar (i-k)-esimo menor porque perdemos a referencia da particao esquerda
        return randomized_select(A, q + 1, r, i - k);
    }
}

// NOTE: diferentemente do partition, em que o pivo eh sempre o ultimo elemento,
// partition_around recebe como parametro quem vai ser o pivo
int partition_around(int A[], int p, int r, int x) {
    int pivot_index = -1;
    
    // encontrar a posicao real do valor x no subvetor
    for (int k = p; k <= r; k++) {
        if (A[k] == x) {
            pivot_index = k;
            break;
        }
    }
    
    // move o pivo para o final (posicao r)
    if (pivot_index != -1) {
        swap(&A[pivot_index], &A[r]);
    }

    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            swap(&A[i], &A[j]);
        }
    }

    // coloca o pivo na sua posicao definitiva
    swap(&A[i+1], &A[r]);

    return i + 1;
}

int median_select(int A[], int p, int r, int i) {

    int j, g, x, q, k;

    while ((r - p + 1) % 5 != 0) {
        // coloque o minimo no comeco da particao
        for (j = p + 1; j <= r; j++) {
            if (A[p] > A[j]) {
                // troque o proximo (j) com o anterior (p)
                swap(&A[j], &A[p]);
            }
        }

        // i=1 ja corresponde ao minimo (estatistica de ordem 1)
        if (i == 1) return A[p];

        // deslocamos a particao (p + 1) para posicionar o proximo menor valor (rank i-1)
        p = p + 1;
        i = i - 1;
    }

    // grupo de 5 elementos
    g = (r - p + 1)/5;

    for (j = p; j <= p + g - 1; j++) {
        int* G = (int*) calloc(5, sizeof(int));
        for (int n = 0; n < 5; n++) {
            G[n] = A[j + n*g];
        }

        // ordene os elementos do grupo
        qsort(G, 5, sizeof(int), compare);

        // copie-os novamente para A nas suas posicoes corretas
        for (int n = 0; n < 5; n++) {
            A[j + n*g] = G[n];
        }
        
        free(G);
    }

    // procure o pivo entre a segunda e terceira parte (middle fifth) dos grupos
    // NOTE: a mediana esta posicionada entre essas duas partes
    x = median_select(A, p + 2*g, p + 3*g - 1, (g + 1)/2);
    q = partition_around(A, p, r, x);
    k = q - p + 1;

    if (i == k) {
        return A[q];
    } else if (i < k) {
        return median_select(A, p, q - 1, i);
    } else {
        return median_select(A, q + 1, r, i - k);
    }

}