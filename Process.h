/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <sys/types.h>

// A strucutre that defines a process which will be used in myshell
// Length and max length of Array of command also is recorded for quick access
typedef struct Process {
	pid_t pid; // The initial pid of the process
	char** commands; // The commands to be run, order is followed by piping order
	int background, timeX; // Flags to determine if the process should be running in background or as a timeX command
	unsigned length, maxLength;
} Process;

void Process_init(Process* process, char* input, int background, int timeX); // Initialize a process by passing in the input, background flag and timeX flag
void Process_destruct(Process* process); // Destruct the process by freeing up memory
