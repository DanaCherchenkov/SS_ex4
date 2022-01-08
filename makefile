CC=gcc
AR=ar
FLAGS= -Wall -g 
LM = -lm

all: graph

graph: algo.o libgraph.a
	$(CC) $(FLAGS) -o graph algo.o libgraph.a 

libgraph.a: algo.o 
	$(AR) -rcs libgraph.a algo.o

algo.o: algo.c 
	$(CC) $(FLAGS) -c algo.c


.PHONY: clean all

clean:
	rm -f *.o *.a graph
