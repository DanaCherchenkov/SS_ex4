CC=gcc
AR=ar
FLAGS= -Wall -g 
LM = -lm

all: graph

graph: main.o libgraph.a
	$(CC) $(FLAGS) -o graph main.o libgraph.a 

libgraph.a: graph.o 
	$(AR) -rcs libgraph.a graph.o

graph.o: graph.c graph.h
	$(CC) $(FLAGS) -c graph.c

main.o: main.c graph.h
	$(CC) $(FLAGS) -c main.c


.PHONY: clean all

clean:
	rm -f *.o *.a graph
