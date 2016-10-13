CC = gcc
CFLAGS = -Wall

all: myshell

myshell: myshell.c getInput PidArray Process utils parser ProcStat
	$(CC) -c myshell.c $(CFLAGS)
	$(CC) -o $@ myshell.o getInput.o PidArray.o Process.o utils.o parser.o ProcStat.o $(CFLAGS)

getInput: getInput.c getInput.h
	gcc -c $@.c $(CFLAGS)

PidArray: PidArray.c PidArray.h
	gcc -c $@.c $(CFLAGS)

Process: Process.c Process.h
	gcc -c $@.c $(CFLAGS)

utils: utils.c utils.h
	gcc -c $@.c $(CFLAGS)

parser: parser.c parser.h
	gcc -c $@.c $(CFLAGS)

ProcStat: ProcStat.c ProcStat.h
	gcc -c $@.c $(CFLAGS)

clean:
	rm -f myshell myshell.o getInput.o PidArray.o Process.o utils.o parser.o ProcStat.o
