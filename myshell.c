#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "PidArray.h"
#include "ProcStat.h"
#include "Process.h"
#include "getInput.h"
#include "utils.h"
#include "parser.h"

void newProcess(Process* process);
void execute(char* command);
void ctrlCAction();
void childProcExit();

static pid_t foregroundPid = 0;
static PidArray timeXPids;

int main() {
	signal(SIGINT, ctrlCAction);
	signal(SIGCHLD, childProcExit);
	PidArray_init(&timeXPids);
	while (1) {
		printf("## myshell $ ");
		char *input = getInput(10);
		if (strncmp(input, "exit", 4) == 0) {
			if (strlen(input) == 4) {
				free(input);
				break;
			} else {
				printf("myshell: \"exit\" with other arguments!!!\n");
				continue;
			}
		}
		int background = isBackground(input);
		int timeX = isTimeX(input);
		if (input[0] == '\0') {
			free(input);
			continue;
		}

		Process process;
		Process_init(&process, input, background, timeX);

		newProcess(&process);

		Process_destruct(&process);

		free(input);
	}
	PidArray_destruct(&timeXPids);
	return 0;
}

void newProcess(Process* process) {
	pid_t pid = fork();

	if (pid < 0) {
		printf("fork: error no = %s\n", strerror(errno));
		exit(-1);
	} else if (pid == 0) {
		int i, in = 0, fd[2];
		for (i = 0; i < process->length - 1; i++) {
			pipe(fd);
			pid_t childPid = fork();
			if (childPid < 0) {
				exit(-1);
			} else if (childPid == 0) {
				if (in != 0) {
					dup2(in, 0);
				}
				if (fd[1] != 1) {
					dup2(fd[1], 1);
				}
				execute(process->commands[i]);
			} else {
				if (process->timeX == 1) {
					PidArray_insert(&timeXPids, childPid);
				}
				if (process->background == 0) {
					foregroundPid = childPid;
				}
				siginfo_t infop;
				int waitidResult = waitid(P_PID, childPid, &infop, WNOWAIT | WEXITED);
				if (waitidResult < 0) {
					printf("Errno: %d\n", errno);
				}
				foregroundPid = 0;
			}
			close(fd[1]);
			in = fd[0];
		}
		if (in != 0) {
			dup2(in, 0);
		}
		execute(process->commands[i]);
	} else {
		if (process->timeX == 1) {
			PidArray_insert(&timeXPids, pid);
		}
		if (process->background == 0) {
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

void execute(char* command) {
	char** arg = parseExec(command);
	if (execvp(arg[0], arg) == -1) {
		printf("myshell: '%s': %s\n", arg[0], strerror(errno));
		exit(-1);
	}
	exit(0);
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
		int timeXPidIndex = PidArray_indexOf(&timeXPids, pid);
		if (timeXPidIndex != -1) {
			ProcStat *stat = getProcStat(pid);
			printProcStat(stat);
			PidArray_delete(&timeXPids, timeXPidIndex);
		}
		if (pid != foregroundPid) {
			ProcStat *stat = getProcStat(pid);
			printf("Program terminated.\n");
			if (stat != NULL) {
				printf("[%d] %s Done\n", (int) stat->pid, stat->comm);
				free(stat);
			}
		}
		waitpid(pid, &status, 0);
	}
}
