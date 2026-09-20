# referencia: https://www.geeksforgeeks.org/dsa/radix-sort/

def radix_sort(arr):

    # o maior elemento define a quantidade de digitos (d)
    # NOTE: "d" define quantas vezes o counting sort vai ser chamado
    # entao, o radix sort eh O(n * d)
    max1 = max(arr)

    # max1 / exp vai possibilitar a leitura dos digitos da direita para a esquerda
    exp = 1
    while max1 / exp >= 1:
        counting_sort(arr, exp)
        exp *= 10


def counting_sort(arr, exp1):

    n = len(arr)

    output = [0] * (n)

    # cria o array de contagem
    # NOTE: algarismos de 0 a 9 (10 elementos)
    count = [0] * (10)

    # registra a frequencia dos ultimos digitos de cada elemento do array de entrada
    for i in range(0, n):
        index = arr[i] // exp1
        # N % 10 extrai o ultimo digito de N
        count[index % 10] += 1

    # soma cumulativa sobre o array de contagem
    for i in range(1, 10):
        count[i] += count[i - 1]

    # posiciona os elementos ordenados com base no seu digito menos significativo
    i = n - 1
    while i >= 0:
        index = arr[i] // exp1
        # a ordenacao se baseia no digito mais a direita
        output[count[index % 10] - 1] = arr[i]
        count[index % 10] -= 1
        i -= 1

    # copia o array de saida
    i = 0
    for i in range(0, len(arr)):
        arr[i] = output[i]
