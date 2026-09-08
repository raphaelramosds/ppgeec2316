import random

import pytest

from multm import mult_matriz_quadradas

METODOS = ["iterativo", "recursivo", "recursivo-strassen"]


def _mult_referencia(a, b):
    n = len(a)
    return [
        [sum(a[i][k] * b[k][j] for k in range(n)) for j in range(n)]
        for i in range(n)
    ]


def _gerar_matriz(n, seed):
    rng = random.Random(seed)
    return [[rng.uniform(-10, 10) for _ in range(n)] for _ in range(n)]


def _matrizes_iguais(a, b, tol=1e-9):
    n = len(a)
    return all(abs(a[i][j] - b[i][j]) < tol for i in range(n) for j in range(n))


@pytest.mark.parametrize("metodo", METODOS)
def test_matriz_1x1(metodo):
    a = [[3.0]]
    b = [[4.0]]

    assert mult_matriz_quadradas(a, b, metodo) == [[12.0]]


@pytest.mark.parametrize("metodo", METODOS)
def test_matriz_2x2_valor_conhecido(metodo):
    a = [[1.0, 2.0], [3.0, 4.0]]
    b = [[5.0, 6.0], [7.0, 8.0]]

    # esperado calculado manualmente: A*B
    esperado = [[19.0, 22.0], [43.0, 50.0]]

    assert _matrizes_iguais(mult_matriz_quadradas(a, b, metodo), esperado)


@pytest.mark.parametrize("metodo", METODOS)
@pytest.mark.parametrize("n", [1, 2, 4, 8, 16])
def test_contra_multiplicacao_referencia(metodo, n):
    a = _gerar_matriz(n, seed=n * 17 + 1)
    b = _gerar_matriz(n, seed=n * 31 + 2)

    esperado = _mult_referencia(a, b)
    resultado = mult_matriz_quadradas(a, b, metodo)

    assert _matrizes_iguais(resultado, esperado)


@pytest.mark.parametrize("n", [1, 2, 4, 8, 16])
def test_metodos_concordam_entre_si(n):
    a = _gerar_matriz(n, seed=n * 5 + 3)
    b = _gerar_matriz(n, seed=n * 7 + 4)

    resultados = [mult_matriz_quadradas(a, b, metodo) for metodo in METODOS]

    for resultado in resultados[1:]:
        assert _matrizes_iguais(resultados[0], resultado)


@pytest.mark.parametrize("metodo", METODOS)
def test_matriz_identidade(metodo):
    n = 4
    identidade = [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
    a = _gerar_matriz(n, seed=99)

    assert _matrizes_iguais(mult_matriz_quadradas(a, identidade, metodo), a)


def test_dimensoes_incompativeis_lanca_excecao():
    a = [[1.0, 2.0], [3.0, 4.0]]
    b = [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]]

    with pytest.raises(Exception):
        mult_matriz_quadradas(a, b, "iterativo")


def test_metodo_invalido_lanca_excecao():
    a = [[1.0]]
    b = [[1.0]]

    with pytest.raises(Exception):
        mult_matriz_quadradas(a, b, "metodo-inexistente")
