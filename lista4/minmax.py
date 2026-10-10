def validar_tamanho(a: list) -> int:
    n = len(a)
    if n == 0:
        raise ValueError("O vetor não pode estar vazio.")
    return n

def minmax_nao_simultaneo(a: list) -> tuple[float, int | float]:
    n = validar_tamanho(a)

    # inicializa min e max como o primeiro elemento
    min_val = a[0]
    max_val = a[0]

    i = 1
    while i < n:
        if a[i] < min_val:
            min_val = a[i]
        if a[i] > max_val:
            max_val = a[i]
        i += 1

    return min_val, max_val

def minmax_simultaneo(a: list) -> tuple[int | float, int | float]:
    n = validar_tamanho(a)

    # se n for impar, inicializa min e max como o primeiro elemento
    if n % 2 == 1:
        min_val = a[0]
        max_val = a[0]
        i = 1
    # se n for par, comparar os dois primeiros elementos
    else:
        min_val = a[0] if a[0] < a[1] else a[1] 
        max_val = a[0] if a[0] > a[1] else a[1] 
        # iterador comeca em 2 pq ja comparamos os dois primeiros
        i = 2

    while i < n - 1: # n-1 porque estamos iterando de 2 em 2
        # calcular minimo e maximo entre os pares i e i+1
        local_min = a[i] if a[i] < a[i+1] else a[i+1]
        local_max = a[i] if a[i] > a[i+1] else a[i+1]

        # comparar com o resultado global
        min_val = local_min if local_min < min_val else min_val
        max_val = local_max if local_max > max_val else max_val

        # ir para o proximo par
        i += 2

    return min_val, max_val