CXX=gcc

CXXFLAGS=-O3 -fopenmp --std=gnu99

LDLIBS=-lmpascalops

TARGETS= \
	pfib_main.out \
	fib_main.out \
	pmatvec_main.out \
	multm_main.out \
	pmultm_main.out

%.out: %.c
	$(CXX) $< -o $@ $(CXXFLAGS) $(LDLIBS)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)