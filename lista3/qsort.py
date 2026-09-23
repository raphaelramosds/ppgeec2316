import random

def quick_sort(v, left, right):
    if left < right:
        pivot_index = partition(v, left, right)
        quick_sort(v, left, pivot_index - 1)
        quick_sort(v, pivot_index + 1, right)


def quick_sort_random(v, left, right):
    if left < right:
        pivot_index = partition_random(v, left, right)
        quick_sort_random(v, left, pivot_index - 1)
        quick_sort_random(v, pivot_index + 1, right)


# o indice do pivo fica no fim da particao
def partition(v, left, right):
    # posicao final do pivo: comeca no inicio do array 
    # ("avanca" sempre que um el menor que ele eh encontrado)
    new_pindex = left

    pivot = v[right]
    for i in range(left, right):
        # avancamos new_pindex enquanto encontrarmos el menor que o pivo
        if v[i] < pivot:
            v[i], v[new_pindex] = v[new_pindex], v[i]
            new_pindex += 1

    # posiciona pivo na posicao correta
    v[new_pindex], v[right] = v[right], v[new_pindex]

    # neste ponto, os els menores que o pivo estao a sua esquerda
    # e os maiores a sua direita
    return new_pindex


# o indice do pivo eh escolhido aleatoriamente
def partition_random(v, left, right):

    # Gera um indice aleatorio entre left e right
    random_index = random.randrange(left, right)

    # Coloca o elemento v[random_index] no fim
    v[random_index], v[right] = v[right], v[random_index]

    # Segue normalmente
    new_pindex = left
    pivot = v[right]
    for i in range(left, right):
        if v[i] < pivot:
            v[i], v[new_pindex] = v[new_pindex], v[i]
            new_pindex += 1
    v[new_pindex], v[right] = v[right], v[new_pindex]
    return new_pindex