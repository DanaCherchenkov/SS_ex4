CC=gcc
AR=ar
FLAGS= -Wall -g 
LM = -lm

all: graph

graph: libgraph.a
	$(CC) $(FLAGS) -o graph libgraph.a 

libgraph.a: algo.o 
	$(AR) -rcs libgraph.a algo.o

algo.o: algo.c algo.h
	$(CC) $(FLAGS) -c algo.c


.PHONY: clean all

clean:
	rm -f *.o *.a graph
