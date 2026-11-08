CXX=gcc

CXXFLAGS=-O3 -fopenmp --std=c99

TARGETS= \
	pfib_main.out \
	fib_main.out \
	pmatvec_main.out \

%.out: %.c
	$(CXX) $< -o $@ $(CXXFLAGS) $(LDFLAGS)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)