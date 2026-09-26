from enum import Enum
from math import sqrt
from dataclasses import dataclass

INF = 1.0e16

class Label(Enum):
    ACCEPTED = 1
    CONSIDERED = 2
    FAR = 3

@dataclass
class Node:
    label: Label = Label.FAR
    val: float = INF

def eikonal_solver(grid: list[list[Node]], i: int, j: int, v: float, tamanho: int) -> float:
    h = 1.0  # espacamento da malha
    
    # encontrar o menor valor aceito na direcao horizontal (Ux)
    ux = INF
    if i > 0 and grid[i-1][j].label == Label.ACCEPTED:
        ux = min(ux, grid[i-1][j].val)
    if i < tamanho - 1 and grid[i+1][j].label == Label.ACCEPTED:
        ux = min(ux, grid[i+1][j].val)

    # encontra o menor valor aceito na direcao vertical (Uy)
    uy = INF
    if j > 0 and grid[i][j-1].label == Label.ACCEPTED:
        uy = min(uy, grid[i][j-1].val)
    if j < tamanho - 1 and grid[i][j+1].label == Label.ACCEPTED:
        uy = min(uy, grid[i][j+1].val)

    # solucao unidimensional caso a condicao de existencia da solucao 2D nao for satisfeita)
    u_1d = min(ux + h / v, uy + h / v)

    # checagem do caso 2D (delta >= 0)
    # NOTE que ux e uy podem nao ser atualizados se nenhum vizinho for aceito
    if ux < INF and uy < INF:
        # condicao de existencia da solucao 2D: |Ux - Uy| < h / v
        if abs(ux - uy) < (h / v):
            delta = (ux + uy)**2 - 2.0 * (ux**2 + uy**2 - (h / v)**2)
            if delta >= 0.0:
                u_2d = 0.5 * (ux + uy) + 0.5 * sqrt(delta)
                return u_2d

    # retorna a atualizacao 1D como fallback
    return u_1d

def atualizar_vizinhos(i: int, j: int, grid: list[list[Node]], vel: list[list[float]], tamanho: int) -> None:
    di = [-1, 1, 0, 0] # vizinhos horizontais (i-1 e i+1)
    dj = [0, 0, -1, 1] # vizinhos verticais (j-1 e j+1)

    for k in range(4):
        ni = i + di[k]
        nj = j + dj[k]

        # Verifica limites do grid
        if 0 <= ni < tamanho and 0 <= nj < tamanho:
            
            # apenas atualiza se ainda nao forem aceitos
            if grid[ni][nj].label != Label.ACCEPTED:
                novo_val = eikonal_solver(grid, ni, nj, vel[ni][nj], tamanho)
                
                if novo_val < grid[ni][nj].val:
                    grid[ni][nj].val = novo_val
                
                grid[ni][nj].label = Label.CONSIDERED

def matriz_tempos_chegada(grid: list[list[Node]]) -> list[list[float]]:
    n = len(grid)
    tempos = []
    for i in range(n):
        linha = []
        for j in range(n):
            linha.append(grid[i][j].val)
        tempos.append(linha)
    return tempos

def fmm(n, sx, sy) -> list[list[Node]]:
    """
    *Fast Marching Method* executado em um grid n x n grid, a partir
    de um modelo de velocidade constante. Cada node (i,j) do grid representa
    o tempo U(i,j) de primeira chegada da onda

    Parametros:

    n: tamanho do grid
    sx, sy: coordenadas da fonte (source)

    Retorna:
    
    Matriz final com os tempos de chegada que resolvem a *Equacao Diferencia Eikonal*
    """
    # modelo de velocidade constante (0.3 km/s)
    f = [[0.3 for _ in range(n)] for _ in range(n)]

    # matriz de tempos de primeira chegada da onda
    u_grid = [[Node(label=Label.FAR, val=INF) for _ in range(n)] for _ in range(n)]

    # definir posicao da fonte
    u_grid[sx][sy].val = 0.0 # tempo de chegada nulo
    u_grid[sx][sy].label = Label.ACCEPTED
    atualizar_vizinhos(sx, sy, u_grid, f, n)

    while True:
        existem_considered = False
        min_val = INF
        min_i = -1
        min_j = -1

        # encontra node "considered" com menor tempo de chegada 
        for i in range(n):
            for j in range(n):
                # OBS: no pior caso, se essa verificacao, que custa O(n^2), for feita para todos os M nodes da matriz
                # teremos um custo computacional de O(M * n^2). Como M = n * n, entao a complexidade total eh O(n^4)
                if u_grid[i][j].label == Label.CONSIDERED:
                    existem_considered = True
                    if u_grid[i][j].val < min_val:
                        min_val = u_grid[i][j].val
                        min_i = i
                        min_j = j

        # se nao ha mais nodes "considered", o algoritmo terminou
        if not existem_considered:
            break

        # marcar o menor node como aceito
        u_grid[min_i][min_j].label = Label.ACCEPTED

        # atualizar os vizinhos do node recem aceito
        atualizar_vizinhos(min_i, min_j, u_grid, f, n)

    return matriz_tempos_chegada(u_grid)