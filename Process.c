#include <stdlib.h>

#include "Process.h"

void Process_init(Process* process) {
	process->commands = NULL;
	process->length = 0;
	process->maxLength = 5;
	Process_update(process);
}

void Process_update(Process* process) {
	process->commands = realloc(process->commands, sizeof(char**) * process->maxLength);
}

void Process_destruct(Process* process) {
	free(process->commands);
	process->length = 0;
	process->maxLength = 0;
}
