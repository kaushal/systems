CC=gcc
CFLAGS=-Wall -g

build:
	make myMalloc.o myMalloc

myMalloc: myMalloc.o
	$(CC) $(CFLAGS) myMalloc.o -o myMalloc

myMalloc.o: myMalloc.c
	$(CC) $(CFLAGS) -c myMalloc.c

clean:
	-rm -rf *.o myMalloc *.dSYM

