/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void StringArray_destruct(char** stringArray) {
	// Free every string until null is detected, free itself at last
	int i = 0;
	while (stringArray[i] != NULL) {
		free(stringArray[i]);
		i++;
	}
	free(stringArray);
}

int sizeOfDynamic(char *input) {
	// Get the size of the string by determining the length characters before a null string occur
	int output = 0;
	while (*input++ != '\0') {
		output++;
	}
	return output;
}

char *timeToString(unsigned long time) {
	// Get the time in second by dividing the system clock tick
	double timeInSecond = time * 1.0f / sysconf(_SC_CLK_TCK);
	char *timeInString = malloc(255 * sizeof(char));
	// Get the time with 2 decimal place in string
	sprintf(timeInString, "%.2lf s", timeInSecond);
	return timeInString;
}
