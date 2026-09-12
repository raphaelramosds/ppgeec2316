def insertion_sort_ram(lista):
    # ignoramos as operacoes das inicializacoes
    n = len(lista)
    indices_lista = range(1, n)
    operacoes = 0

    # operacoes contam a partir daqui
    for i in indices_lista:
        # 1 op: acessar posicao de memoria i em indices_lista
        # 1 op: calcular i + 1
        # 1 op: atribuir i
        operacoes += 3

        chave = lista[i]
        # 1 op: acessar posicao de memoria i em lista
        # 1 op: atribuir chave
        operacoes += 2

        j = i - 1
        # 1 op: calcular i - 1
        # 1 op: atribuir j
        operacoes += 2

        while j >= 0 and lista[j] > chave:
            # 1 op: calcular j >= 0
            # 1 op: acessar posicao de memoria j em lista
            # 1 op: ler chave
            # 1 op: calcular lista[j] > chave
            # 1 op: calcular valor lógico do AND
            operacoes += 5

            lista[j + 1] = lista[j]
            # 1 op: calcula 1 + 1
            # 1 op: acessa posicao de memoria j em lista
            # 1 op: acessa posicao de memoria j + 1 em lista
            # 1 op: atribuir lista[j] em lista[j + 1]
            operacoes += 4

            j -= 1
            # 1 op: calcular j - 1
            # 2 op: atribuir j
            operacoes += 2

        # 5 op: avaliacao de saida do while
        operacoes += 5

        # 1 op: calcular j + 1
        # 1 op: acessa posicao de memoria j + 1 em lista
        # 1 op: ler chave
        # 1 op: atribuir chave em lista[j+1]
        lista[j + 1] = chave
        operacoes += 4

    # 3 op: avaliacao de saida do for
    operacoes += 3

    return lista, operacoes