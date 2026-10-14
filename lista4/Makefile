# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 
TARGETS = selection_main selection_test minmax_main postoffice_main
OPT=-O2

# Regra padrao (executada ao digitar apenas 'make'): compila todos os executaveis
all: $(TARGETS)

selection_main: selection_main.c selection.c selection.h utils.h
	$(CC) $(CFLAGS) $(OPT) selection_main.c selection.c -o $@

selection_test: selection_test.c selection.c selection.h utils.h
	$(CC) $(CFLAGS) $(OPT) selection_test.c selection.c -o $@

minmax_main: minmax_main.c minmax.h
	$(CC) $(CFLAGS) $(OPT) $< -o $@

postoffice_main: postoffice_main.c weighted_median.h
	$(CC) $(CFLAGS) $(OPT) $< -o $@

# Atalhos com hifen mantidos por compatibilidade
selection-main: selection_main
selection-test: selection_test

# Regra para limpar arquivos gerados (executaveis)
clean:
	rm -f $(TARGETS)

.PHONY: all clean selection-main selection-test
