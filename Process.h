#include <sys/types.h>

typedef struct Process {
	pid_t pid;
	char** commands;
	unsigned length, maxLength;
} Process;

void Process_init(Process* process);
void Process_update(Process* process);
void Process_destruct(Process* process);
