/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <stdio.h>
#include <stdlib.h>

char *getInput(size_t size){
	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char)*size);
	if (!str) return str;
	while (EOF != (ch = fgetc(stdin)) && ch != '\n'){
		str[len++]=ch;
		if(len==size){
			str = realloc(str, sizeof(char)*(size+=16));
			if(!str)return str;
		}
	}
	str[len++]='\0';
	return realloc(str, sizeof(char)*len);
}
