def inicializar_matriz_resultado(a,b):
    n_a = len(a)
    n_b = len(b)

    if n_a != n_b:
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

def dividir_submatrizes(m):
    n = len(m)
    meio = n // 2

    m11 = [linha[:meio] for linha in m[:meio]]
    m12 = [linha[meio:] for linha in m[:meio]]
    m21 = [linha[:meio] for linha in m[meio:]]
    m22 = [linha[meio:] for linha in m[meio:]]

    return m11, m12, m21, m22

def somar_matrizes(a, b):
    n = len(a)
    return [[a[i][j] + b[i][j] for j in range(n)] for i in range(n)]

def subtrair_matrizes(a, b):
    n = len(a)
    return [[a[i][j] - b[i][j] for j in range(n)] for i in range(n)]
