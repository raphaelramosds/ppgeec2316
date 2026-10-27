# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = estruturas_main test_hash_search test_hash_insert test_hash_remove test_hash_remove_insert test_hash_sondagem_tempo test_hash_sondagem_colisoes
OPT=-O1

SRCS = array_ordenado.c lista_encadeada.c rubro_negra.c
HDRS = array_ordenado.h lista_encadeada.h rubro_negra.h

HASH_SRCS = hash.c hash_aberto.c hash_encadeamento.c lista_encadeada.c
HASH_HDRS = hash.h hash_aberto.h hash_encadeamento.h lista_encadeada.h test_hash_utils.h

# Regra padrao (executada ao digitar apenas 'make'): compila todos os executaveis
all: $(TARGETS)

estruturas_main: estruturas_main.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OPT) estruturas_main.c $(SRCS) -o $@

test_hash_%: test_hash_%.c $(HASH_SRCS) $(HASH_HDRS)
	$(CC) $(CFLAGS) $(OPT) $< $(HASH_SRCS) -o $@ -lm

# Regra para limpar arquivos gerados (executaveis)
clean:
	rm -f $(TARGETS)

.PHONY: all clean
