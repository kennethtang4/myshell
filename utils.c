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
