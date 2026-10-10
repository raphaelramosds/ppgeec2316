#ifndef MINMAX_H
#define MINMAX_H

#include <stdio.h>
#include <stdlib.h>

// Estrutura para simular o retorno de uma tupla (min, max)
typedef struct {
    double min_val;
    double max_val;
} resultado;

// Validacao de tamanho equivalente a funcao em Python
void validar_tamanho(int n) {
    if (n <= 0) {
        fprintf(stderr, "Erro: O vetor nao pode estar vazio.\n");
        exit(EXIT_FAILURE);
    }
}

// Abordagem Nao-Simultanea
resultado minmax_nao_simultaneo(const double *a, int n) {
    validar_tamanho(n);
    
    resultado res;
    res.min_val = a[0];
    res.max_val = a[0];

    int i = 1;
    while (i < n) {
        if (a[i] < res.min_val) {
            res.min_val = a[i];
        }
        if (a[i] > res.max_val) {
            res.max_val = a[i];
        }
        i++;
    }

    return res;
}

// Abordagem Simultanea
resultado minmax_simultaneo(const double *a, int n) {
    validar_tamanho(n);
    
    resultado res;
    int i;

    // Se n for impar, inicializa min e max com o primeiro elemento
    if (n % 2 == 1) {
        res.min_val = a[0];
        res.max_val = a[0];
        i = 1;
    } 
    // Se n for par, compara os dois primeiros elementos
    else {
        if (a[0] < a[1]) {
            res.min_val = a[0];
            res.max_val = a[1];
        } else {
            res.min_val = a[1];
            res.max_val = a[0];
        }
        i = 2;
    }

    // Processa o restante dos elementos em pares
    while (i < n - 1) {
        double local_min, local_max;
        if (a[i] < a[i+1]) {
            local_min = a[i];
            local_max = a[i+1];
        } else {
            local_min = a[i+1];
            local_max = a[i];
        }

        // Compara com o resultadoado global
        if (local_min < res.min_val) {
            res.min_val = local_min;
        }
        if (local_max > res.max_val) {
            res.max_val = local_max;
        }

        i += 2;
    }

    return res;
}

#endif