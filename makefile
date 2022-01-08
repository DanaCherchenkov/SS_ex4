CC=gcc
AR=ar
FLAGS= -Wall -g 
LM = -lm

all: graph

graph: algo.o libgraph.a
	$(CC) $(FLAGS) -o graph algo.o libgraph.a 
	
algo.o: algo.c algo.h
	$(CC) $(FLAGS) -c algo.c


libgraph.a: algo.o 
	$(AR) -rcs libgraph.a algo.o

.PHONY: clean all

clean:
	rm -f *.o *.a graph
