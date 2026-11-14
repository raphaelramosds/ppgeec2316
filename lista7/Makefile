CXX=gcc

CXXFLAGS=-O3 -fopenmp --std=gnu99 -I$(PASCAL_DIR)/include
# instalacao do PaScal (sobrescreva com: make PASCAL_DIR=/outro/caminho)
PASCAL_DIR?=$(HOME)/.local/pascal-releases-master

LDFLAGS=-L$(PASCAL_DIR)/lib -Wl,-rpath,$(PASCAL_DIR)/lib
LDLIBS=-lmpascalops

TARGETS= \
	pfib_main.out \
	fib_main.out \
	pmatvec_main.out \
	multm_main.out \

%.out: %.c
	$(CXX) $< -o $@ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)