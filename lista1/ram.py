def insertion_sort_ram(lista):
    # NOTE: cada linha tem custo unitario, contado a cada execucao (c1 = c2 = ... = 1)
    # NOTE: estrutura "for" substituido pelo "while" para ter maior
    # controle nas operacoes de verificacao de indice
    n = len(lista)
    operacoes = 0

    j = 1
    while True:
        operacoes += 1
        if j >= n:
            break

        chave = lista[j]
        operacoes += 1

        i = j - 1
        operacoes += 1

        while True:
            operacoes += 1
            if not (i >= 0 and lista[i] > chave):
                break
            lista[i + 1] = lista[i]
            operacoes += 1
            i -= 1
            operacoes += 1

        lista[i + 1] = chave
        operacoes += 1

        j += 1

    return lista, operacoes
