/*update for the process manager*/

#include <sys/types.h>

typedef struct Process {
	pid_t pid;
	char** commands;
	int background, timeX;
	unsigned length, maxLength;
} Process;

void Process_init(Process* process, char* input, int background, int timeX);
void Process_destruct(Process* process);
