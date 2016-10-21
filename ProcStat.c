#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "ProcStat.h"
#include "utils.h"

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

int isStringDigit(char* string) {
	int i = 0;
	while (string[i] != '\0') {
		if (!isdigit(string[i++])) {
				return 0;
		}
	}
	return 1;
}

int searchChild(pid_t pid, int padding) {
	DIR *dir;
	struct dirent *ent;
	int count = 0;
	if ((dir = opendir ("/proc")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (isStringDigit(ent->d_name)) {
				pid_t childPid = (pid_t) strtol(ent->d_name, (char **)NULL, 10);
				ProcStat *ps = getProcStat(childPid);
				if (ps->ppid == pid) {
					if (count > 0) {
						int i;
						for (i = 0; i < padding; i++) {
							printf(" ");
						}
					}
					printf(" - %s", ps->comm);
					if (searchChild(childPid, padding + 3 + strlen(ps->comm)) == 0) {
						printf("\n");
					}
					count++;
				}
			}
		}
		closedir (dir);
	} else {
		printf("error: could not read the proc directory\n");
	}
	return count;
}

void printProcStat(ProcStat *stat) {
	if (stat != NULL) {
		char *realTimeString, *userTimeString, *systemTimeString;
		realTimeString = timeToString(stat->utime + stat->stime);
		userTimeString = timeToString(stat->utime);
		systemTimeString = timeToString(stat->stime);
		printf("\n");
		printf("%s\t%-15s\t%-10s\t%-10s\t%-10s\n", "PID", "CMD", "RTIME", "UTIME", "STIME");
		printf("%d\t%-15s\t%-10s\t%-10s\t%-10s\n", (int) stat->pid, stat->comm, realTimeString, userTimeString, systemTimeString);
		free(realTimeString);
		free(userTimeString);
		free(systemTimeString);
	}
}

void printViewTree() {
	char name[7] = "myshell";
	printf("%s", name);
	if (searchChild(getpid(), strlen(name)) == 0) {
		printf("\n");
	}
}
