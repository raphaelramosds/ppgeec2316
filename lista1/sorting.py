def insertion_sort(lista):
    n = len(lista)
    indices_lista = range(1, n)
    
    for i in indices_lista:
        chave = lista[i]
        j = i - 1
        while j >= 0 and lista[j] > chave:
            lista[j + 1] = lista[j]
            j -= 1
        lista[j + 1] = chave

    return lista

def _merge(lista, inicio, meio, fim):
    esquerda = lista[inicio:meio + 1]
    direita = lista[meio + 1:fim + 1]

    i = j = 0
    k = inicio
    while i < len(esquerda) and j < len(direita):
        if esquerda[i] <= direita[j]:
            lista[k] = esquerda[i]
            i += 1
        else:
            lista[k] = direita[j]
            j += 1
        k += 1

    while i < len(esquerda):
        lista[k] = esquerda[i]
        i += 1
        k += 1

    while j < len(direita):
        lista[k] = direita[j]
        j += 1
        k += 1

def _merge_sort(lista, inicio, fim):
    if inicio < fim:
        meio = (inicio + fim) // 2
        _merge_sort(lista, inicio, meio)
        _merge_sort(lista, meio + 1, fim)
        _merge(lista, inicio, meio, fim)

def merge_sort(lista):
    _merge_sort(lista, 0, len(lista) - 1)
    return lista
