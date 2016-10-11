all: myshell

myshell: myshell.c
	gcc $^ -o $@ -Wall

clean:
	rm -f myshell 
