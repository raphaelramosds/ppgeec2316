# referencia: https://www.geeksforgeeks.org/dsa/radix-sort/

def radix_sort(arr):

    # Encontra o maior numero para saber a quantidade de digitos
    max1 = max(arr)

    # Faz o counting sort para cada digito. Note que, em vez de
    # passar o numero do digito, passa-se exp. exp e 10^i,
    # onde i e o numero do digito atual
    exp = 1
    while max1 / exp >= 1:
        counting_sort(arr, exp)
        exp *= 10


def counting_sort(arr, exp1):

    n = len(arr)

    # O array de saida que vai conter arr ordenado
    output = [0] * (n)

    # inicializa o array de contagem com 0
    # NOTE: algarismos de 0 a 9 (10 elementos)
    count = [0] * (10)

    # Armazena a quantidade de ocorrencias em count[]
    for i in range(0, n):
        index = arr[i] // exp1
        count[index % 10] += 1

    # Altera count[i] para que count[i] passe a conter a posicao
    # real desse digito no array de saida
    for i in range(1, 10):
        count[i] += count[i - 1]

    # Constroi o array de saida
    i = n - 1
    while i >= 0:
        index = arr[i] // exp1
        output[count[index % 10] - 1] = arr[i]
        count[index % 10] -= 1
        i -= 1

    # Copia o array de saida para arr[],
    # de modo que arr agora contem os numeros ordenados
    i = 0
    for i in range(0, len(arr)):
        arr[i] = output[i]
