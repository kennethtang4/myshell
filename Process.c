#include <stdlib.h>

#include "Process.h"
#include "parser.h"

void Process_init(Process* process, char* input, int background, int timeX) {
	process->commands = parseInput(input);
	int length = 0;
	while (process->commands[length++] != NULL);
	length--;
	process->background = background;
	process->timeX = timeX;
	process->length = length;
	process->maxLength = length;
}

void Process_destruct(Process* process) {
	free(process->commands);
	process->length = 0;
	process->maxLength = 0;
}
