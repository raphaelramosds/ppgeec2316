import numpy as np
import matplotlib.pyplot as plt
from fmm import fmm, INF

# TODO se optimize = True, entao chama a implementacao por priority queue
def call_fmm(n, sx, sy, optimize=False) -> None:
    # chama algoritmo
    u_grid = fmm(n, sx, sy)

    # parse dos nodes: fmm retorna uma unica matriz n x n de tempos
    np_u_grid = np.array(u_grid, dtype=float)

    # Substitui o valor de node nao visitado (INF) por NaN para o plot
    np_u_grid[np_u_grid >= INF] = np.nan

    # Visualiza o estado final
    plt.imshow(np_u_grid, cmap="viridis", origin="lower")
    plt.colorbar(label="Tempo de Chegada (s)")
    plt.title("Mapa de Tempos de Chegada (FMM)")
    plt.show()

    return np_u_grid
