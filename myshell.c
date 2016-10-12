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
#include "parser.h"

void StringArray_destruct(char** stringArray);

void ctrlCAction();
void childProcExit();

static pid_t foregroundPid = 0;

int main() {
	signal(SIGINT, ctrlCAction);
	signal(SIGCHLD, childProcExit);
	ProcessArray processArray;
	int i = 0;
	while (1) {
		printf("## myshell $ ");
		char *input = getInput(10);
		if (input[0] == '\0') {
			continue;
		}
		if (strcmp(input, "exit") == 0) {
			break;
		}
		int background = 0;
		i = 0;
		while (input[i++] != '\0');
		i -= 2;
		if (input[i] == '&' && input[i - 1] == ' ') {
			background = 1;
			input[i - 1] = '\0';
		}
		char** processes = parseInput(input);
		char** arg = parseExec(input);
		if (strcmp(arg[0], "timeX") == 0) {
			for (i = 0; i < sizeof(arg) - 1; i++) {
				arg[i] = arg[i + 1];
			}
		}

		pid_t pid = fork();
	
		if (pid < 0) {
			printf("fork: error no = %s\n", strerror(errno));
			exit(-1);
		} else if (pid == 0) {
			if (execvp(arg[0], arg) == -1) {
				printf("myshell: '%s': %s\n", arg[0], strerror(errno));
				exit(-1);
			}
			exit(0);
		} else {
			if (background == 0) {
				foregroundPid = pid;

				siginfo_t infop;
				int waitidResult = waitid(P_PID, pid, &infop, WNOWAIT | WEXITED);
      
				if (waitidResult < 0) {
					printf("Errno: %d\n", errno);
				}

				foregroundPid = 0;
			}
		}

		StringArray_destruct(processes);
		StringArray_destruct(arg);
	}
	return 0;
}

void StringArray_destruct(char** stringArray) {
	int i = 0;
	while (stringArray[i] != NULL) {
		free(stringArray[i]);
		i++;
	}
	free(stringArray);
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
