from multm_helpers import (
    inicializar_matriz_resultado,
    dividir_submatrizes,
    somar_matrizes,
    subtrair_matrizes,
)

def multm_iterativo(a,b):
    n, c = inicializar_matriz_resultado(a,b)

    # realizar multiplicacao
    for i in range(n):
        for j in range(n):
            soma = 0
            for k in range(n):
                soma = soma + a[i][k]*b[k][j]
            c[i][j] = soma

    return c

def _multm_recursivo(
    a,
    b,
    c,
    linha_a,
    col_a,
    linha_b,
    col_b,
    linha_c,
    col_c,
    n
    ):

    # caso base: submatrizes 1x1 (acumula o produto direto em C)
    if n == 1:
        c[linha_c][col_c] += a[linha_a][col_a] * b[linha_b][col_b]
        return

    meio = n // 2

    # c11 = A11*B11 + A12*B21
    _multm_recursivo(a, b, c, linha_a, col_a, linha_b, col_b, linha_c, col_c, meio) # A11*B11
    _multm_recursivo(a, b, c, linha_a, col_a + meio, linha_b + meio, col_b, linha_c, col_c, meio) # A12*B21

    # c12 = A11*B12 + A12*B22
    _multm_recursivo(a, b, c, linha_a, col_a, linha_b, col_b + meio, linha_c, col_c + meio, meio) # A11*B12
    _multm_recursivo(a, b, c, linha_a, col_a + meio, linha_b + meio, col_b + meio, linha_c, col_c + meio, meio) # A12*B22

    # c21 = A21*B11 + A22*B21
    _multm_recursivo(a, b, c, linha_a + meio, col_a, linha_b, col_b, linha_c + meio, col_c, meio) # A21*B11
    _multm_recursivo(a, b, c, linha_a + meio, col_a + meio, linha_b + meio, col_b, linha_c + meio, col_c, meio) # A22*B21

    # c22 = A21*B12 + A22*B22
    _multm_recursivo(a, b, c, linha_a + meio, col_a, linha_b, col_b + meio, linha_c + meio, col_c + meio, meio) # A21*B12
    _multm_recursivo(a, b, c, linha_a + meio, col_a + meio, linha_b + meio, col_b + meio, linha_c + meio, col_c + meio, meio) # A22*B22

def multm_recursivo(a, b):
    n, c = inicializar_matriz_resultado(a, b)
    _multm_recursivo(a, b, c, 0, 0, 0, 0, 0, 0, n)
    return c

def multm_strassen(a, b):
    n = len(a)

    # caso base: submatrizes 1x1
    if n == 1:
        return [[a[0][0] * b[0][0]]]

    a11, a12, a21, a22 = dividir_submatrizes(a)
    b11, b12, b21, b22 = dividir_submatrizes(b)

    # 7 multiplicacoes recursivas (produtos de Strassen)
    m1 = multm_strassen(somar_matrizes(a11, a22), somar_matrizes(b11, b22))
    m2 = multm_strassen(somar_matrizes(a21, a22), b11)
    m3 = multm_strassen(a11, subtrair_matrizes(b12, b22))
    m4 = multm_strassen(a22, subtrair_matrizes(b21, b11))
    m5 = multm_strassen(somar_matrizes(a11, a12), b22)
    m6 = multm_strassen(subtrair_matrizes(a21, a11), somar_matrizes(b11, b12))
    m7 = multm_strassen(subtrair_matrizes(a12, a22), somar_matrizes(b21, b22))

    # combinacao dos produtos nos blocos de C
    c11 = somar_matrizes(subtrair_matrizes(somar_matrizes(m1, m4), m5), m7)
    c12 = somar_matrizes(m3, m5)
    c21 = somar_matrizes(m2, m4)
    c22 = somar_matrizes(subtrair_matrizes(somar_matrizes(m1, m3), m2), m6)

    # juntar submatrizes c11, c12, c21, c22 na matriz resultado c
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