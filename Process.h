/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <sys/types.h>

typedef struct Process {
	pid_t pid;
	char** commands;
	int background, timeX;
	unsigned length, maxLength;
} Process;

void Process_init(Process* process, char* input, int background, int timeX);
void Process_destruct(Process* process);
