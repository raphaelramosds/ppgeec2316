CC=gcc

# -fno-stack-protector: evita a leitura do canario (TLS) dentro do VALGRIND_PRINTF
CFLAGS=-O0 -g -fno-stack-protector -fopenmp --std=gnu99

# dimensao das matrizes usada no traco (potencia de 2)
N ?= 8

# arquivos de saida do traco de memoria
#   RAW   traco completo do lackey (loader, libc, init, ...)
#   TRACE apenas a regiao entre TRACE_BEGIN e TRACE_END (I/L/S/M)
#   DATA  apenas acessos de dados as matrizes A, B e C (sem I e sem pilha 1ff...)
TRACE ?= multm_n$(N).trace
RAW   ?= $(TRACE:.trace=.raw.trace)
DATA  ?= $(TRACE:.trace=.data.trace)

TARGETS= \
	multm_main.out \
	lru_main.out

%.out: %.c
	$(CC) $< -o $@ $(CFLAGS)

all: $(TARGETS)

lru_main.out: lru_main.c cache_lru.c cache.h
	$(CC) lru_main.c cache_lru.c -o $@ $(CFLAGS)

# atalho: make lru-main
lru-main: lru_main.out

# extrai o traco de memoria com o lackey do valgrind e recorta a regiao
# delimitada por VALGRIND_PRINTF("TRACE_BEGIN") / VALGRIND_PRINTF("TRACE_END")
# NOTE: sudo apt install valgrind
# NOTE: formato de cada linha do traco do lackey: <tipo> <endereco_hex>,<bytes>
#   I  busca de instrucao (instruction fetch)
#   L  leitura de dado da memoria (load)
#   S  escrita de dado na memoria (store)
#   M  leitura seguida de escrita no mesmo endereco (modify, ex.: x += 1)
# ex.: " L 0403c6e0,8" = leitura de 8 bytes (um double) no endereco 0x0403c6e0
trace-multm: multm_main.out
	valgrind --tool=lackey --trace-mem=yes --log-file=$(RAW) ./multm_main.out $(N)
	sed -n '/TRACE_BEGIN/,/TRACE_END/{/TRACE_/d;p}' $(RAW) > $(TRACE)
# mantem apenas L/S/M (linhas de dados comecam com espaco) e descarta
# a pilha (enderecos 1ff...), restando os acessos as matrizes A, B e C
	grep -E '^ [LSM] ' $(TRACE) | grep -v ' 1ff' > $(DATA)

clean:
	rm -f $(TARGETS) *.trace

.PHONY: all clean trace-multm lru-main
