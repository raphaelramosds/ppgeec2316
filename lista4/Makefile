# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O1
TARGET = minmax_main postoffice_main selection_test selection_main

# Regra padrao (executada ao digitar apenas 'make')
all: $(TARGET)

# Regra de compilacao do executavel a partir do arquivo fonte
$(TARGET): $(TARGET)_main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

selection-test: selection_test.c selection.c selection.h utils.h
	$(CC) $(CFLAGS) selection_test.c selection.c -o selection_test

selection-main: selection_main.c selection.c selection.h utils.h
	$(CC) $(CFLAGS) selection_main.c selection.c -o selection_main

# Regra para limpar arquivos gerados (executavel)
clean:
	rm -f $(TARGET) selection_test

.PHONY: all clean run selection