CC=gcc
CFLAGS=-Wall -g

build:
	make main.o queue.o main

main: main.o queue.o
	$(CC) $(CFLAGS) main.o queue.o -o main -l pthread

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

clean:
	-rm -rf *.o main *.dSYM

