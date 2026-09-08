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

    # c12 = A11*B12 + A12*B22
    _recursivo(a, b, c, idx_la, idx_ca, idx_lb, idx_cb + meio, idx_lc, idx_cc + meio, meio) # A11*B12
    _recursivo(a, b, c, idx_la, idx_ca + meio, idx_lb + meio, idx_cb + meio, idx_lc, idx_cc + meio, meio) # A12*B22

    # c21 = A21*B11 + A22*B21
    _recursivo(a, b, c, idx_la + meio, idx_ca, idx_lb, idx_cb, idx_lc + meio, idx_cc, meio) # A21*B11
    _recursivo(a, b, c, idx_la + meio, idx_ca + meio, idx_lb + meio, idx_cb, idx_lc + meio, idx_cc, meio) # A22*B21

    # c22 = A21*B12 + A22*B22
    _recursivo(a, b, c, idx_la + meio, idx_ca, idx_lb, idx_cb + meio, idx_lc + meio, idx_cc + meio, meio) # A21*B12
    _recursivo(a, b, c, idx_la + meio, idx_ca + meio, idx_lb + meio, idx_cb + meio, idx_lc + meio, idx_cc + meio, meio) # A22*B22

def _dividir_submatrizes(m):
    n = len(m)
    meio = n // 2

    m11 = [linha[:meio] for linha in m[:meio]]
    m12 = [linha[meio:] for linha in m[:meio]]
    m21 = [linha[:meio] for linha in m[meio:]]
    m22 = [linha[meio:] for linha in m[meio:]]

    return m11, m12, m21, m22

def _somar_matrizes(a, b):
    n = len(a)
    return [[a[i][j] + b[i][j] for j in range(n)] for i in range(n)]

def _subtrair_matrizes(a, b):
    n = len(a)
    return [[a[i][j] - b[i][j] for j in range(n)] for i in range(n)]

def _juntar_submatrizes(c11, c12, c21, c22):
    meio = len(c11)
    n = meio * 2
    c = [[0.0] * n for _ in range(n)]

    for i in range(meio):
        for j in range(meio):
            c[i][j] = c11[i][j]
            c[i][j + meio] = c12[i][j]
            c[i + meio][j] = c21[i][j]
            c[i + meio][j + meio] = c22[i][j]

    return c

def _recursivo_strassen(a, b):
    n = len(a)

    # caso base: submatrizes 1x1
    if n == 1:
        return [[a[0][0] * b[0][0]]]

    a11, a12, a21, a22 = _dividir_submatrizes(a)
    b11, b12, b21, b22 = _dividir_submatrizes(b)

    # 7 multiplicacoes recursivas (produtos de Strassen)
    m1 = _recursivo_strassen(_somar_matrizes(a11, a22), _somar_matrizes(b11, b22))
    m2 = _recursivo_strassen(_somar_matrizes(a21, a22), b11)
    m3 = _recursivo_strassen(a11, _subtrair_matrizes(b12, b22))
    m4 = _recursivo_strassen(a22, _subtrair_matrizes(b21, b11))
    m5 = _recursivo_strassen(_somar_matrizes(a11, a12), b22)
    m6 = _recursivo_strassen(_subtrair_matrizes(a21, a11), _somar_matrizes(b11, b12))
    m7 = _recursivo_strassen(_subtrair_matrizes(a12, a22), _somar_matrizes(b21, b22))

    # combinacao dos produtos nos blocos de C
    c11 = _somar_matrizes(_subtrair_matrizes(_somar_matrizes(m1, m4), m5), m7)
    c12 = _somar_matrizes(m3, m5)
    c21 = _somar_matrizes(m2, m4)
    c22 = _somar_matrizes(_subtrair_matrizes(_somar_matrizes(m1, m3), m2), m6)

    return _juntar_submatrizes(c11, c12, c21, c22)

def mult_matriz_quadradas(a, b, metodo=''): # metodo pode ser iterativo, recursivo, recursivo-strassen
    n, c = _inicializar_matriz_resultado(a,b)

    if metodo == 'iterativo':
        return _iterativo(a, b)
    elif metodo == 'recursivo':
        _recursivo(a, b, c, 0, 0, 0, 0, 0, 0, n)
        return c
    elif metodo == 'recursivo-strassen':
        return _recursivo_strassen(a, b)
    else:
        raise Exception("Metodo invalido: use 'iterativo', 'recursivo' ou 'recursivo-strassen'")