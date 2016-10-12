CC = gcc
CFLAGS = -Wall

all: myshell

myshell: myshell.c getInput ProcessArray utils parser ProcStat
	$(CC) -c myshell.c $(CFLAGS)
	$(CC) -o $@ myshell.o getInput.o ProcessArray.o utils.o parser.o ProcStat.o $(CFLAGS)

getInput: getInput.c getInput.h
	gcc -c $@.c $(CFLAGS)

ProcessArray: ProcessArray.c ProcessArray.h Process.h
	gcc -c $@.c $(CFLAGS)

utils: utils.c utils.h
	gcc -c $@.c $(CFLAGS)

parser: parser.c parser.h
	gcc -c $@.c $(CFLAGS)

ProcStat: ProcStat.c ProcStat.h
	gcc -c $@.c $(CFLAGS)

clean:
	rm -f myshell myshell.o getInput.o ProcessArray.o utils.o parser.o ProcStat.o
