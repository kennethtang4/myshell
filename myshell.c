#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ProcStat.h"
#include "ProcessArray.h"
#include "getInput.h"
#include "utils.h"
#include "parser.h"

void newProcess(Process* process);
void ctrlCAction();
void childProcExit();

static pid_t foregroundPid = 0;

int main() {
	signal(SIGINT, ctrlCAction);
	signal(SIGCHLD, childProcExit);
	ProcessArray processArray;
	ProcessArray_init(&processArray);
	while (1) {
		printf("## myshell $ ");
		char *input = getInput(10);
		if (strcmp(input, "exit") == 0) {
			break;
		}
		int background = isBackground(input);
		int timeX = isTimeX(input);
		if (input[0] == '\0') {
			continue;
		}

		Process process;
		Process_init(&process, input, background, timeX);

		newProcess(&process);

		Process_destruct(&process);
	}
	ProcessArray_destruct(&processArray);
	return 0;
}

void newProcess(Process* process) {
	pid_t pid = fork();

	if (pid < 0) {
		printf("fork: error no = %s\n", strerror(errno));
		exit(-1);
	} else if (pid == 0) {
		char** arg = parseExec(process->commands[0]);
		if (execvp(arg[0], arg) == -1) {
			printf("myshell: '%s': %s\n", arg[0], strerror(errno));
			exit(-1);
		}
		exit(0);
	} else {
		if (process->background == 0) {
			if (process->timeX == 1) {
			}
			foregroundPid = pid;

			siginfo_t infop;
			int waitidResult = waitid(P_PID, pid, &infop, WNOWAIT | WEXITED);
    
			if (waitidResult < 0) {
				printf("Errno: %d\n", errno);
			}

			foregroundPid = 0;
		}
	}
}

void ctrlCAction() {
	printf("Receives SIGINT!! IGNORE IT :)\n");
}

void childProcExit() {
	siginfo_t infop;
	int waitidResult;
	int status;
	while ((waitidResult = waitid(P_ALL, 0, &infop, WNOWAIT | WEXITED)) >= 0) {
		pid_t pid = infop.si_pid;
		if (pid != foregroundPid) {
			ProcStat *stat = getProcStat(pid);
			printf("Program terminated.\n");
			if (stat != NULL) {
				printf("[%d] %s Done\n", (int) stat->pid, stat->comm);
				//printProcStat(stat);
				free(stat);
			}
		}
		waitpid(pid, &status, 0);
	}
}
