CC=gcc
CFLAGS=-Wall -g

build:
	make main.o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

main: main.o
	$(CC) $(CFLAGS) main.o -o main

clean:
	-rm -rf *.o main *.dSYM

