CC=gcc
CFLAGS=-Wall -g

build:
	make search.o search

search.o: search.c
	$(CC) $(CFLAGS) -c search.c

search: search.o
	$(CC) $(CFLAGS) search.o -o search

clean:
	-rm -rf *.o search *.dSYM

rebuild:
	clean build

