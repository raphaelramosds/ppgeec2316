def permutar(a: list):
    import random

    n = len(a)
    
    for i in range(n):
        # a cada elemento visitado, escolha outro aleatoriamente, e troque suas posicoes
        j = random.randint(i, n - 1)
        a[i], a[j] = a[j], a[i]
    return a


def distribuir(tarefas: list, k: int):
    import time

    n = len(tarefas)

    # dividimos as particoes usando a carga "exata"
    tamanho_carga = n // k
    particoes = [tarefas[i * tamanho_carga : (i + 1) * tamanho_carga] for i in range(k)]

    # (k * tamanho_carga) eh a ultima posicao da ultima particao
    resto = tarefas[k * tamanho_carga :]
    if resto:
        # se ainda houver elementos a partir dessa posicao, adicionamos eles na ultima particao
        particoes[-1].extend(resto)

    # chamar trabalhadores
    resultados = []
    for particao in particoes:
        inicio = time.perf_counter()
        for carga_ms in particao:
            time.sleep(carga_ms / 1000)
        fim = time.perf_counter()
        resultados.append(fim - inicio)

    # retornamos o tempo de execucao do trabalhador que foi sobrecarregado
    return max(resultados)


if __name__ == "__main__":
    import random

    # fixamos a semente para que as permutacoes sejam sempre as mesmas
    random.seed(42)

    # cada tarefa eh dada em milissegundos
    tarefas = [100, 90, 90, 80, 80, 10, 10, 100]
    n_permutacoes = 10

    # numero de trabalhadores
    k = 3

    carga_max = distribuir(tarefas, k)
    print(f"lista original {tarefas} -> carga maxima: {carga_max * 1000:.1f} ms ")

    for i in range(n_permutacoes):
        permutada = permutar(list(tarefas))
        carga_max = distribuir(permutada, k)
        print(f"permutacao {i + 1} {permutada} -> carga maxima: {carga_max * 1000:.1f} ms ")