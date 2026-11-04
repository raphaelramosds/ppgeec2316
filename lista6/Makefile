# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = ospf_main maze_main dag_main
OPT=-O1

SRCS = grafo.c heap.c conjuntos.c dfs.c mst.c dijkstra.c labirinto.c
HDRS = grafo.h heap.h conjuntos.h dfs.h mst.h dijkstra.h labirinto.h

# Regra padrao (executada ao digitar apenas 'make'): compila todos os executaveis
all: $(TARGETS)

ospf_main: ospf_main.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OPT) ospf_main.c $(SRCS) -o $@ -lm

maze_main: maze_main.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OPT) maze_main.c $(SRCS) -o $@ -lm

dag_main: dag_main.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OPT) dag_main.c $(SRCS) -o $@ -lm

# Regra para limpar arquivos gerados (executaveis)
clean:
	rm -f $(TARGETS)

.PHONY: all clean
