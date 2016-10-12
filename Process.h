#include <sys/types.h>

typedef struct Process {
	pid_t pid;
	char** commands;
	unsigned length, maxLength;
} Process;
