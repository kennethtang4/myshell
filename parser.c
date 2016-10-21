/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int isBackground(char* input) {
	// Check if the last character is '&'.
	// If yes then remove the character and return 1 else return 0.
	int i = sizeOfDynamic(input), j;
	for (j = 0; j < i - 2; j++) {
		if (input[j] == '&') {
			return - 1;
		}
	}
	if (input[i - 1] == '&') {
		input[i - 1] = '\0';
		return 1;
	}
	return 0;
}

int isTimeX(char *input) {
	int i = sizeOfDynamic(input), j;
	i--;
	// The input must not be a timeX command as it has less characters than timeX
	if (i < 4) {
		return 0;
	}
	char timeXString[5] = "timeX";
	for (j = 0; j < 5; j++) {
		if (input[j] != timeXString[j]) {
			// If any character is not equal to timeX then it is not a timeX command
			return 0;
		}
	}
	// Allow extra spacing between timeX and the actual command
	if (input[j] == ' ') {
		while (input[j++] == ' ');
	}
	if (input[j] == '\0') {
		// If there is no command, then timeX cannot run as itself only
		printf("myshell: \"timeX\" cannot be a standalone command\n");
		input[0] = '\0';
		return 0;
	}
	if (input[5] != ' ') {
		// It is just a command starts with the characters timeX but not the acutal timeX
		return 0;
	}
	for (j = 0; j < i - 5; j++) {
		// It is confirmed as a timeX command
		// Characters are shifted to remove timeX from the command
		input[j] = input[j + 6];
	}
	for (j = i - 5; j < i; j++) {
		// Just to be sure that the string is null after the last character
		input[j] = '\0';
	}
	return 1;
}

char **parseInput(char *input) {
	int numberOfElements = 5;
	// The output will be a dynamic array with an initialize size of 5
	char **output = malloc(numberOfElements * sizeof(char*));
	char *start = input;
	char *p = strstr(start, " | "); // Find the first pipe
	int i = 0;
	while (p != NULL) { // Keep finding pipes until none can be found
		// If the dynamic array does not have enough space,
		// expand the array by 5 extra space
		if ((i + 2) >= numberOfElements) {
			numberOfElements += 5;
			output = realloc(output, numberOfElements * sizeof(char*));
		}
		// Identify the length of the command
		int length = p - start;
		// Allocate an address for the command
		output[i] = malloc((length + 1) * sizeof(char));
		// Copy the command to the array
		strncpy(output[i], start, length);
		// Close the command with a null for c string
		// This is important as unpredictable result will occur without this
		output[i++][length] = '\0';
		// Find the next pipe starting from the last position + lenght of " | "
		start += length + 3;
		p = strstr(start, " | ");
	}
	// Check the length of the last command of the string
	int length = sizeOfDynamic(start);
	// Similar to above, allocate memory and copy the command
	output[i] = malloc((length + 1) * sizeof(char));
	strncpy(output[i], start, length);
	output[i++][length] = '\0';
	// The last item of the array should be null as an identification for end of iteration
	output[i++] = NULL;
	return output;
}

char **parseExec(char *exec) {
	int numberOfElements = 5;
	// The output will be a dynamic array with an initialize size of 5
	char **output = malloc(numberOfElements * sizeof(char*));
	// Find the next argument by finding space
	// Argument with space inside will not work at current version
	// Single qoute and double quotes will also not work at current version
	char *p = strtok(exec, " ");
	int i = 0;
	while (p != NULL) {
		// If the dynamic array does not have enough space,
		// expand the array by 5 extra space
		if ((i + 1) >= numberOfElements) {
			numberOfElements += 5;
			output = realloc(output, numberOfElements * sizeof(char*));
		}
		// Identify the length of the argument
		int length = sizeOfDynamic(p);
		// Allocate an address for the argument
		output[i] = malloc((length + 1) * sizeof(char));
		// Copy the argument to the array
		strcpy(output[i++], p);
		// Find the next argument
		p = strtok(NULL, " ");
	}
	output[i++] = NULL;
	return output;
}
