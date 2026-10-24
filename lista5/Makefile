# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = estruturas_main
OPT=-O1

SRCS = array_ordenado.c lista_encadeada.c rubro_negra.c
HDRS = array_ordenado.h lista_encadeada.h rubro_negra.h

# Regra padrao (executada ao digitar apenas 'make'): compila todos os executaveis
all: $(TARGETS)

estruturas_main: estruturas_main.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OPT) estruturas_main.c $(SRCS) -o $@

# Regra para limpar arquivos gerados (executaveis)
clean:
	rm -f $(TARGETS)

.PHONY: all clean
