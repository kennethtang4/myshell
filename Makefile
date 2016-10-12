CC = gcc
CFLAGS = -Wall

all: myshell

myshell: myshell.c ProcessArray utils parser
	$(CC) -c myshell.c $(CFLAGS)
	$(CC) -o $@ myshell.o ProcessArray.o utils.o parser.o $(CFLAGS)

ProcessArray: ProcessArray.c ProcessArray.h Process.h
	gcc -c $@.c $(CFLAGS)

utils: utils.c utils.h
	gcc -c $@.c $(CFLAGS)

parser: parser.c parser.h
	gcc -c $@.c $(CFLAGS)

clean:
	rm -f myshell myshell.o ProcessArray.o utils.o parser.o
