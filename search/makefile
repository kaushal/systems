CC=gcc
CFLAGS=-Wall -g

build:
	make main.o tokenizer.o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

main: main.o tokenizer.o
	$(CC) $(CFLAGS) main.o tokenizer.o -o main

clean:
	-rm -rf *.o main *.dSYM

rebuild:
	clean build

