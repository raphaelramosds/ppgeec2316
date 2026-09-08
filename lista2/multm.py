def mult_matriz_quad(a,b):
    n_a = len(a)
    n_b = len(b)

    if n_a is not n_b:
        raise Exception("Matrizes devem ter o mesmo numero de linhas e colunas")

    n = n_b
    matriz_resultado = []

    # preencher matriz resultados com zero
    for _ in range(n):
        linha = []
        for _ in range(n):
            linha.append(0.0)
        matriz_resultado.append(linha)

    # realizar multiplicacao
    for i in range(n_b):
        for j in range(n):
            soma = 0
            for k in range(n):
                soma = soma + a[i][k]*b[k][j]
            matriz_resultado[i][j] = soma

    return matriz_resultado

def _submatriz_quadrada(a, n_elementos):
    ...

def mult_matriz_quad_rec(a, b):
    ...