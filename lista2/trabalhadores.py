import random
import time


def permutar_aleatoriamente(a):
    n = len(a)
    for i in range(n):
        j = random.randint(i, n - 1)
        a[i], a[j] = a[j], a[i]
    return a


def particionar_sequencial(tarefas, k):
    n = len(tarefas)
    tamanho = n // k

    particoes = [tarefas[i * tamanho:(i + 1) * tamanho] for i in range(k)]
    resto = tarefas[k * tamanho:]
    if resto:
        particoes[-1].extend(resto)

    return particoes


def trabalhador(tarefas):
    inicio = time.perf_counter()
    for carga_ms in tarefas:
        time.sleep(carga_ms / 1000)
    return time.perf_counter() - inicio


def executar_balanceador(tarefas, k):
    particoes = particionar_sequencial(tarefas, k)
    resultados = [trabalhador(particao) for particao in particoes]

    return max(resultados), resultados


def demonstrar_impacto_permutacao(tarefas, k, n_permutacoes=5):
    carga_max, cargas = executar_balanceador(list(tarefas), k)
    print(f"ordem original {tarefas} -> carga maxima: {carga_max * 1000:.1f} ms "
          f"(cargas: {[round(c * 1000, 1) for c in cargas]})")

    for i in range(n_permutacoes):
        permutada = permutar_aleatoriamente(list(tarefas))
        carga_max, cargas = executar_balanceador(permutada, k)
        print(f"permutacao {i + 1} {permutada} -> carga maxima: {carga_max * 1000:.1f} ms "
              f"(cargas: {[round(c * 1000, 1) for c in cargas]})")


if __name__ == "__main__":
    # cada tarefa eh dada em milissegundos 
    tarefas = [100, 10, 10, 50, 50, 10, 10, 100]
    demonstrar_impacto_permutacao(tarefas, k=4)
