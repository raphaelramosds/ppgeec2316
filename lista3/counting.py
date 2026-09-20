def counting_sort(v):

    # Encontra o menor e o maior elemento
    lowest = v[0]
    highest = v[0]
    for i in range(1, len(v)):
        if v[i] > highest:
            highest = v[i]
        if v[i] < lowest:
            lowest = v[i]

    # Cria o array de contagem
    c = [0] * (highest - lowest + 1)

    # Preenche o array de contagem com a frequencia de cada elemento
    for i in range(0, len(v)):
        c[v[i] - lowest] += 1

    # Soma cumulativa sobre o array de contagem
    for i in range(1, len(c)):
        c[i] += c[i - 1]

    # Cria o array ordenado
    s = [0] * len(v)

    # Encontra a posicao equivalente de v[i] no array c e insere no array s
    for i in range(0, len(v)):
        s[c[v[i] - lowest] - 1] = v[i]
        c[v[i] - lowest] -= 1

    # Copia o array
    for i in range(0, len(v)):
        v[i] = s[i]