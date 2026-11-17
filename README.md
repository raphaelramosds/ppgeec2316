# PPGEEC2316 — Análise e Projeto de Algoritmos

Resoluções das listas de exercícios da disciplina, com implementações próprias e experimentos numéricos baseados no livro *Introduction to Algorithms* (Cormen et al.). As listas 1 a 3 foram feitas em Python (com notebooks Jupyter) e as listas 4 a 7 em C.

| Lista | Questões | Tema | Linguagem |
|-------|----------|------|-----------|
| [Lista 1](#lista-1--fundamentos-e-ordenação) | 1–3 | Corretude, modelo RAM, insertion sort × merge sort | Python |
| [Lista 2](#lista-2--divisão-e-conquista-e-recorrências) | 4–6 | Strassen, recorrências, balanceamento de carga | Python |
| [Lista 3](#lista-3--heaps-quicksort-e-ordenação-linear) | 7–9 | Fila de prioridade (aplicada ao Fast Marching Method), quicksort, radix/counting sort | Python |
| [Lista 4](#lista-4--estatísticas-de-ordem) | 10–12 | Seleção, mínimo e máximo, mediana ponderada | C |
| [Lista 5](#lista-5--estruturas-de-dados-e-tabelas-hash) | 13–15 | Array, lista encadeada, rubro-negra, tabelas hash | C |
| [Lista 6](#lista-6--grafos) | 16–18 | Ordenação topológica, MST (Kruskal/Prim), Dijkstra | C |
| [Lista 7](#lista-7--paralelismo-com-openmp) | 19–21 | Paralelismo com OpenMP e análise com PaScal Analyzer | C + OpenMP |

---

## Lista 1 — Fundamentos e ordenação

Experimentos em [`lista1/Lista1-Experimentos.ipynb`](lista1/Lista1-Experimentos.ipynb).

- **Q1.** Pseudocódigo da busca linear e prova de corretude por invariante de laço (Exercício 2.1-3).
- **Q2.** Insertion sort anotado para contar operações no modelo RAM e comparação com o tempo medido em uma máquina real.
- **Q3.** Insertion sort × merge sort no melhor caso, no pior caso e no caso médio, buscando o ponto de cruzamento entre os dois.

| Arquivo | Conteúdo |
|---------|----------|
| `sorting.py` | `insertion_sort`, `merge_sort` |
| `ram.py` | `insertion_sort_ram`: insertion sort que conta as operações do modelo RAM |

## Lista 2 — Divisão e conquista e recorrências

Experimentos em [`lista2/Lista2-Experimentos.ipynb`](lista2/Lista2-Experimentos.ipynb).

- **Q4.** Multiplicação de matrizes: algoritmo convencional, recursivo e de Strassen (com corte de recursão), mostrando quando Strassen fica mais rápido.
- **Q5.** Resolução da recorrência de um algoritmo recursivo e comparação com o tempo medido.
- **Q6.** Balanceamento de carga com *n* tarefas e *k* trabalhadores: efeito de embaralhar aleatoriamente a entrada sobre a carga máxima.

| Arquivo | Conteúdo |
|---------|----------|
| `multm.py` | `multm_iterativo`, `multm_recursivo`, `multm_strassen` (corte padrão = 64) |
| `multm_helpers.py` | Funções auxiliares: dividir em submatrizes, somar e subtrair matrizes |
| `buscalinear.py` | Busca linear recursiva (usada na análise de recorrência) |
| `trabalhadores.py` | `permutar` (Fisher–Yates) e `distribuir` (simula os trabalhadores com `sleep`) |

## Lista 3 — Heaps, quicksort e ordenação linear

Experimentos em [`lista3/Lista3-Experimentos.ipynb`](lista3/Lista3-Experimentos.ipynb).

- **Q7.** Fila de prioridade mínima (seção 6.5 do Cormen), aplicada ao **Fast Marching Method** para resolver a equação Eikonal em uma malha 2D. A versão com heap é comparada com a versão ingênua.
- **Q8.** Quicksort determinístico × quicksort aleatório: em quais entradas cada um é mais vantajoso.
- **Q9.** Radix sort (com counting sort como sub-rotina) × counting sort sozinho.

| Arquivo | Conteúdo |
|---------|----------|
| `mpqueue.py` | `MinPriorityQueue` (insert, extract-min, decrease-key) |
| `fmm.py` / `fmm_pqueue.py` | Fast Marching Method sem heap e com heap |
| `fmm_helper.py` | Função auxiliar para executar e cronometrar o FMM |
| `qsort.py` | `quick_sort`, `quick_sort_random` |
| `counting.py`, `radix.py` | Counting sort (versão normal e ingênua) e radix sort |
| `FastMarchingMethod.md` | Explicação didática do FMM |
| `SolucaoDiscretaEikonal.md` | Dedução da solução discreta da equação Eikonal |

## Lista 4 — Estatísticas de ordem

Compilação: `cd lista4 && make` (para outra otimização, use `OPT=-O0 make`).

- **Seleção:** `randomized_select` × `median_select` (mediana das medianas), cronometrados com entradas aleatórias e ordenadas.
- **Mínimo e máximo:** cálculo simultâneo (cerca de 3n/2 comparações) × cálculo separado (2n − 2 comparações).
- **Problema da agência postal:** ponto ótimo com distância de Manhattan, encontrado pela mediana ponderada em cada eixo.

| Programa | Descrição |
|----------|-----------|
| `selection_main` | Benchmark dos dois algoritmos de seleção (`selection.c`) |
| `selection_test` | Teste de corretude da seleção contra a lista ordenada |
| `minmax_main` | Benchmark de mínimo e máximo (`minmax.h`; há uma versão em Python em `minmax.py`) |
| `postoffice_main` | Agência postal com mediana ponderada (`weighted_median.h`) |

## Lista 5 — Estruturas de dados e tabelas hash

Compilação: `cd lista5 && make`. Detalhes e resultados em [`lista5/README.md`](lista5/README.md).

- **Q13.** Array ordenado, lista encadeada não ordenada e árvore rubro-negra: tempos de inserção e busca para *n* crescente.
- **Q14.** Hash com encadeamento × endereçamento aberto em busca (com e sem sucesso), inserção, remoção e ciclos de remoção e inserção (efeito das marcas `DELETED`).
- **Q15.** Sondagem linear × hashing duplo, considerando o efeito da cache e o agrupamento primário e secundário.

| Programa | Descrição |
|----------|-----------|
| `estruturas_main` | Q13: `array_ordenado.c`, `lista_encadeada.c`, `rubro_negra.c` |
| `test_hash_search` / `_insert` / `_remove` / `_remove_insert` | Q14: `hash_encadeamento.c` × `hash_aberto.c` |
| `test_hash_sondagem_tempo` / `_colisoes` | Q15: tempo com tabela grande e chaves que colidem em h1 |

## Lista 6 — Grafos

Compilação: `cd lista6 && make`. Os enunciados estão em [`lista6/README.md`](lista6/README.md).

Biblioteca comum: `grafo.c` (lista de adjacência), `dfs.c`, `heap.c` (heap mínimo indexado), `conjuntos.c` (union-find com união por posto), `mst.c` (Kruskal e Prim) e `dijkstra.c`.

| Programa | Questão | Descrição |
|----------|---------|-----------|
| `dag_main` | Q16 | Ordenação topológica (via DFS) do grafo computacional de `f(x, y) = x·y + sin(x)` |
| `maze_main` | Q17 | Gera um labirinto como MST de uma grade e compara o tempo de Kruskal e de Prim |
| `rede_neural_main` | Q17 | MST sobre o grafo de uma rede neural totalmente conectada (grafo denso) e comparação de Kruskal com Prim |
| `ospf_main` | Q18 | Tabela de roteamento OSPF calculada com Dijkstra (custo = banda de referência / banda do enlace) |

As figuras (`dag.svg`, `labirinto_3x3.svg`, `rede_neural_indexacao.svg`, `topologia_ospf.jpg` etc.) ilustram os grafos usados.

## Lista 7 — Paralelismo com OpenMP

Compilação: `cd lista7 && make` (requer `-fopenmp` e a biblioteca `mpascalops`). Os enunciados estão em [`lista7/README.md`](lista7/README.md).

- **Q19.** Relação entre as primitivas *spawn*, *sync* e *parallel for* e o OpenMP (`omp task`, `omp taskwait`, `omp parallel for`).
- **Q20.** Paralelização da multiplicação recursiva de matrizes da Lista 2 com tasks e corte sequencial (`CUTOFF`).
- **Q21.** Análise de *speedup* e eficiência com o PaScal Analyzer, usando 1–32 threads e matrizes de 256 a 4096, em um nó `gpu-4-h200` via SLURM.

| Programa | Descrição |
|----------|-----------|
| `fib_main` / `pfib_main` | Fibonacci sequencial e com `task`/`taskwait` (spawn/sync) |
| `pmatvec_main` | Produto matriz-vetor com `parallel for` |
| `multm_main` / `pmultm_main` | Multiplicação recursiva de matrizes, sequencial e paralela |
| `pmultm_main_job.sh` | Job SLURM que executa o `pascalanalyzer` |
| `Resultados-pmultm_main_job/` | JSON e gráficos de speedup e eficiência |

---

## Como executar

**Python (listas 1–3)**, com [uv](https://docs.astral.sh/uv/) e Python ≥ 3.12:

```bash
uv sync
uv run jupyter lab        # abre os notebooks Lista*-Experimentos.ipynb
make multm                # executa lista2/multm.py
make trabalhadores        # executa lista2/trabalhadores.py
make fmm                  # executa lista3/fmm.py
```

**C (listas 4–7):**

```bash
cd listaN && make         # compila todos os programas da lista
./<programa>
make clean
```
