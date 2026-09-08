def _inicializar_matriz_resultado(a,b):
    n_a = len(a)
    n_b = len(b)

    if n_a is not n_b:
        raise Exception("Matrizes devem ter o mesmo numero de linhas e colunas")

    n = n_b
    matriz_resultado = []

    # preencher matriz resultados com zeros
    for _ in range(n):
        linha = []
        for _ in range(n):
            linha.append(0.0)
        matriz_resultado.append(linha)

    return n, matriz_resultado

def _iterativo(a,b):
    n, c = _inicializar_matriz_resultado(a,b)

    # realizar multiplicacao
    for i in range(n):
        for j in range(n):
            soma = 0
            for k in range(n):
                soma = soma + a[i][k]*b[k][j]
            c[i][j] = soma

    return c

def _recursivo(
    a,
    b,
    c,
    idx_la,
    idx_ca,
    idx_lb,
    idx_cb,
    idx_lc,
    idx_cc,
    n
    ):

    # caso base: submatrizes 1x1 (acumula o produto direto em C)
    if n == 1:
        c[idx_lc][idx_cc] += a[idx_la][idx_ca] * b[idx_lb][idx_cb]
        return

    meio = n // 2

    # c11 = A11*B11 + A12*B21
    _recursivo(a, b, c, idx_la, idx_ca, idx_lb, idx_cb, idx_lc, idx_cc, meio) # A11*B11
    _recursivo(a, b, c, idx_la, idx_ca + meio, idx_lb + meio, idx_cb, idx_lc, idx_cc, meio) # A12*B21

    # TODO complete mantendo os comentarios
    ...

def _recursivo_strassen():
    ...

def mult_matriz_quadradas(a, b, metodo=''): # metodo pode ser iterativo, recursivo, recursivo-strassen
    n, c = _inicializar_matriz_resultado(a,b)

    # TODO complete mantendo os comentarios
    ...