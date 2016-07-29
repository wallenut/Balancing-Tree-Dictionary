CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

Words16: Words16.o protoTree.o getLine.o
	${CC} ${CFLAGS} -o $@ $^ 

Words16.o: Words16.c protoTree.c protoTree.h getLine.c getLine.h
	${CC} ${CFLAGS} -c Words16.c

protoTree.o: protoTree.h 

getLine.o: getLine.h

