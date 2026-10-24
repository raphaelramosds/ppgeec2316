#ifndef ARRAY_ORDENADO_H
#define ARRAY_ORDENADO_H

// Array ordenado com crescimento dinamico (tabela dinamica, CLRS secao 16.4)
typedef struct {
    int *chaves;
    int tamanho;    // numero de elementos armazenados
    int capacidade; // numero de posicoes alocadas
} array_ordenado;

array_ordenado *array_criar(void);
void array_liberar(array_ordenado *a);

// Insere x mantendo o array ordenado: O(lg n) para achar a posicao + O(n) para deslocar
void array_inserir(array_ordenado *a, int x);

// Busca binaria (CLRS exercicio 2.3-6): retorna o indice de x ou -1 se x nao estiver no array
int array_buscar(const array_ordenado *a, int x);

#endif
