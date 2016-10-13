#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int isBackground(char* input) {
	int i = sizeOfDynamic(input);
	if (input[i - 1] == '&' && input[i - 2] == ' ') {
		input[i - 2] = '\0';
		return 1;
	}
	return 0;
}

char **parseInput(char* input) {
	char* temp = malloc(sizeof(char) * sizeOfDynamic(input));
	strcpy(temp, input);
	int numberOfElements = 5;
	char **output = malloc(numberOfElements * sizeof(char*));
	char *p = strtok(temp, " | ");
	int i = 0;
	while (p != NULL) {
		if ((i + 1) >= numberOfElements) {
			numberOfElements += 5;
			output = realloc(output, numberOfElements * sizeof(char*));
		}
		int length = sizeOfDynamic(p);
		output[i] = malloc((length + 1) * sizeof(char));
		strcpy(output[i++], p);
		p = strtok(NULL, " | ");
	}
	output[i++] = NULL;
	free(temp);
	return output;
}

char **parseExec(char* exec) {
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
