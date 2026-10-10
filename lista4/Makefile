# Definicoes do compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3
TARGET = minmax

# Regra padrao (executada ao digitar apenas 'make')
all: $(TARGET)

# Regra de compilacao do executavel a partir do arquivo fonte
$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

# Regra para limpar arquivos gerados (executavel)
clean:
	rm -f $(TARGET)

# Regra para rodar o programa apos compilar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run