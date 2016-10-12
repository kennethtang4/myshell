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
#include "utils.h"
#include "parser.h"

void StringArray_destruct(char** stringArray);

void ctrlCAction();
void childProcExit();
ProcStat *getProcStat(pid_t pid);
void printProcStat(ProcStat *stat);
char *inputString(size_t size);

static pid_t foregroundPid = 0;

int main() {
	signal(SIGINT, ctrlCAction);
	signal(SIGCHLD, childProcExit);
	ProcessArray processArray;
	int i = 0;
	while (1) {
		printf("## myshell $ ");
		char *input = inputString(10);
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

ProcStat *getProcStat(pid_t pid) {
	ProcStat *stat = NULL;
	int i;
	char procStatPath[100];

	FILE *file;
	sprintf(procStatPath, "/proc/%d/stat", (int) pid);
	file = fopen(procStatPath, "r");
	if (file == NULL) {
		printf("Error in open my proc file: %s\n", procStatPath);
	} else {
		stat = malloc(1 * sizeof(ProcStat));
		fscanf(file, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld", &(stat->pid), stat->comm, &(stat->state), &(stat->ppid), &(stat->pgrp), &(stat->session), &(stat->tty_nr), &(stat->tpgid), &(stat->flags), &(stat->minflt), &(stat->cminflt), &(stat->majflt), &(stat->cmajflt), &(stat->utime), &(stat->stime), &(stat->cutime), &(stat->cstime), &(stat->priority), &(stat->nice), &(stat->num_threads), &(stat->itrealvalue), &(stat->starttime), &(stat->vsize), &(stat->rss), &(stat->rsslim), &(stat->startcode), &(stat->endcode), &(stat->startstack), &(stat->kstkesp), &(stat->kstkeip), &(stat->signal), &(stat->blocked), &(stat->sigignore), &(stat->sigcatch), &(stat->wchan), &(stat->nswap), &(stat->cnswap), &(stat->exit_signal), &(stat->processor), &(stat->rt_priority), &(stat->policy), &(stat->delayacct_blkio_ticks), &(stat->guest_time), &(stat->cguest_time));
		fclose(file);

		i = 0;
		while (stat->comm[i] != '\0') {
			stat->comm[i] = stat->comm[i + 1];
			i++;
		}
		stat->comm[i - 2] = '\0';
	}

	return stat;
}

void printProcStat(ProcStat *stat) {
	if (stat != NULL) {
		char *userTimeString, *systemTimeString;
		userTimeString = timeToString(stat->utime);
		systemTimeString = timeToString(stat->stime);
		printf("\n");
		printf("%s\t%-25s\t%-10s\t%-10s\n", "PID", "CMD", "UTIME", "STIME");
		printf("%d\t%-25s\t%-10s\t%-10s\n", (int) stat->pid, stat->comm, userTimeString, systemTimeString);
	}
}

char *inputString(size_t size){
	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char)*size);
	if (!str) return str;
	while (EOF != (ch = fgetc(stdin)) && ch != '\n'){
		str[len++]=ch;
		if(len==size){
			str = realloc(str, sizeof(char)*(size+=16));
			if(!str)return str;
		}
	}
	str[len++]='\0';
	return realloc(str, sizeof(char)*len);
}
