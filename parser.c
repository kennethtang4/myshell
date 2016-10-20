#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int isBackground(char* input) {
	int i = sizeOfDynamic(input);
	if (input[i - 1] == '&') {
		input[i - 1] = '\0';
		return 1;
	}
	return 0;
}

int isTimeX(char *input) {
	int i = sizeOfDynamic(input), j;
	i--;
	if (i < 4) {
		return 0;
	}
	char timeXString[5] = "timeX";
	for (j = 0; j < 5; j++) {
		if (input[j] != timeXString[j]) {
			return 0;
		}
	}
	if (input[j] == ' ') {
		while (input[j++] == ' ');
	}
	if (input[j] == '\0') {
		printf("myshell: \"timeX\" cannot be a standalone command\n");
		input[0] = '\0';
		return 0;
	}
	if (input[5] != ' ') {
		return 0;
	}
	for (j = 0; j < i - 5; j++) {
		input[j] = input[j + 6];
	}
	for (j = i - 5; j < i; j++) {
		input[j] = '\0';
	}
	return 1;
}

char **parseInput(char *input) {
	int numberOfElements = 5;
	char **output = malloc(numberOfElements * sizeof(char*));
	char *start = input;
	char *p = strstr(start, " | ");
	int i = 0;
	while (p != NULL) {
		if ((i + 2) >= numberOfElements) {
			numberOfElements += 5;
			output = realloc(output, numberOfElements * sizeof(char*));
		}
		int length = p - start;
		output[i] = malloc((length + 1) * sizeof(char));
		strncpy(output[i], start, length);
		output[i++][length] = '\0';
		start += length + 3;
		p = strstr(start, " | ");
	}
	int length = sizeOfDynamic(start);
	output[i] = malloc((length + 1) * sizeof(char));
	strncpy(output[i], start, length);
	output[i++][length] = '\0';
	output[i++] = NULL;
	return output;
}

char **parseExec(char *exec) {
	int numberOfElements = 5;
	char **output = malloc(numberOfElements * sizeof(char*));
	char *p = strtok(exec, " ");
	int i = 0;
	while (p != NULL) {
		if ((i + 1) >= numberOfElements) {
			numberOfElements += 5;
			output = realloc(output, numberOfElements * sizeof(char*));
		}
		int length = sizeOfDynamic(p);
		output[i] = malloc((length + 1) * sizeof(char));
		strcpy(output[i++], p);
		p = strtok(NULL, " ");
	}
	output[i++] = NULL;
	return output;
}
