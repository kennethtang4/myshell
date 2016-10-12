CC = gcc
CFLAGS = -Wall

all: myshell

myshell: myshell.c ProcessArray.h ProcessArray.o
	$(CC) -c myshell.c $(CFLAGS)
	$(CC) -o $@ myshell.o ProcessArray.o $(CFLAGS)

ProcessArray: ProcessArray.c
	gcc -c $^ $(CFLAGS)

clean:
	rm -f myshell myshell.o ProcessArray.o
