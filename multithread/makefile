CC=gcc
CFLAGS=-Wall -g

build:
	make main.o tokenizer.o queue.o main

main: main.o tokenizer.o queue.o
	$(CC) $(CFLAGS) main.o tokenizer.o queue.o -o main -l pthread

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

clean:
	-rm -rf *.o main *.dSYM

rebuild:
	make clean build

