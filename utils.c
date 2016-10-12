#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
