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
	int i = 0;
	while (stringArray[i] != NULL) {
		free(stringArray[i]);
		i++;
	}
	free(stringArray);
}

int sizeOfDynamic(char *input) {
	int output = 0;
	while (*input++ != '\0') {
		output++;
	}
	return output;
}

char *timeToString(unsigned long time) {
		double timeInSecond = time * 1.0f / sysconf(_SC_CLK_TCK);
		char *timeInString = malloc(255 * sizeof(char));
		sprintf(timeInString, "%.2lf s", timeInSecond);
		return timeInString;
}
