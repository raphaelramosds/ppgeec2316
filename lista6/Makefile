# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OPT=-O1

SRCS = grafo.c heap.c conjuntos.c dfs.c mst.c dijkstra.c
HDRS = grafo.h heap.h conjuntos.h dfs.h mst.h dijkstra.h
OBJS = $(SRCS:.c=.o)

# Regra padrao (executada ao digitar apenas 'make'): compila os modulos em arquivos objeto
all: $(OBJS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) $(OPT) -c $< -o $@

# Regra para limpar arquivos gerados (objetos)
clean:
	rm -f $(OBJS)

.PHONY: all clean
