def _busca_linear(lista, alvo, idx):
    # caso base: fim da lista, elemento nao encontrado
    if idx == len(lista):
        return -1

    # caso base: elemento encontrado na posicao idx
    if lista[idx] == alvo:
        return idx

    # chamada recursiva avancando para o proximo indice
    return _busca_linear(lista, alvo, idx + 1)

def busca_linear(lista, alvo):
    return _busca_linear(lista, alvo, 0)
