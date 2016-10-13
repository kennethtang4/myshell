#include <stdio.h>
#include <stdlib.h>
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
