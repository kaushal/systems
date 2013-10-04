CC = gcc
CCFLAGS = -w

all: main

main: main.o sorted-list.o
	${CC} ${CCFLAGS} -o main main.o sorted-list.o sorted-list.h
	ar -rv libsl.a sorted-list.o

main.o: main.c
	${CC} ${CCFLAGS} -c main.c

sorted-list.o: sorted-list.c
	${CC} ${CCFLAGS} -c sorted-list.c

clean:
	rm *.o *.a *.gch main
