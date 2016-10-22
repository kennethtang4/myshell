/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

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
	// Get the process statistic
	ProcStat *stat = NULL;
	int i;
	char procStatPath[100];
	FILE *file;
	sprintf(procStatPath, "/proc/%d/stat", (int) pid);
	// Open the process statistic file
	file = fopen(procStatPath, "r");
	if (file == NULL) {
		printf("Error in open my proc file: %s\n", procStatPath);
	} else {
		stat = malloc(1 * sizeof(ProcStat));
		// Putting the process statistic into the structure
		fscanf(file, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld", &(stat->pid), stat->comm, &(stat->state), &(stat->ppid), &(stat->pgrp), &(stat->session), &(stat->tty_nr), &(stat->tpgid), &(stat->flags), &(stat->minflt), &(stat->cminflt), &(stat->majflt), &(stat->cmajflt), &(stat->utime), &(stat->stime), &(stat->cutime), &(stat->cstime), &(stat->priority), &(stat->nice), &(stat->num_threads), &(stat->itrealvalue), &(stat->starttime), &(stat->vsize), &(stat->rss), &(stat->rsslim), &(stat->startcode), &(stat->endcode), &(stat->startstack), &(stat->kstkesp), &(stat->kstkeip), &(stat->signal), &(stat->blocked), &(stat->sigignore), &(stat->sigcatch), &(stat->wchan), &(stat->nswap), &(stat->cnswap), &(stat->exit_signal), &(stat->processor), &(stat->rt_priority), &(stat->policy), &(stat->delayacct_blkio_ticks), &(stat->guest_time), &(stat->cguest_time));
		// Close the process statistic file
		fclose(file);

		// Eliminate the brackets from the name of the process
		i = 0;
		while (stat->comm[i] != '\0') {
			stat->comm[i] = stat->comm[i + 1];
			i++;
		}
		stat->comm[i - 2] = '\0';
	}
	return stat;
}

// Get the uptime of the system from the file "/proc/uptime"
double getUptime() {
	double uptime = 0.0f;
	// The null ending string is important
	// Unpredictable result will occur if omitted
	char uptimePath[13] = "/proc/uptime\0";
	FILE *file;
	// Open the uptime file
	file = fopen(uptimePath, "r");
	if (file == NULL) {
		printf("Error in open uptime file: %s\n", uptimePath);
	} else {
		// Put the uptime into the variable
		fscanf(file, "%lf", &uptime);
		fclose(file);
	}
	return uptime;
}

// Determine if the input string only contains digits (0 - 9)
int isStringDigit(char* string) {
	int i = 0;
	while (string[i] != '\0') {
		if (!isdigit(string[i++])) {
				return 0;
		}
	}
	return 1;
}

// Search for the child process with the pid
// Padding is the space to be appended before the output
// for better visual
int searchChild(pid_t pid, int padding) {
	DIR *dir;
	struct dirent *ent;
	int count = 0;
	// Open the "/proc" directory
	if ((dir = opendir("/proc")) != NULL) {
		// Try the read the directroy
		while ((ent = readdir(dir)) != NULL) {
			// Only read inside of the directory if the directory is a pid number
			if (isStringDigit(ent->d_name)) {
				// Cast the directory name to pid_t
				pid_t childPid = (pid_t) strtol(ent->d_name, (char **)NULL, 10);
				// Get the statistic of the process
				ProcStat *ps = getProcStat(childPid);
				// Check if the parent pid of the child pid is the pid passed in the function
				if (ps != NULL) {
					if (ps->ppid == pid) {
						// Only print the padding if the item is not the first item
						if (count > 0) {
							int i;
							for (i = 0; i < padding; i++) {
								printf(" ");
							}
						}
						// Print the name with " - " for the viewtree
						printf(" - %s", ps->comm);
						// Recursively search for child processes
						if (searchChild(childPid, padding + 3 + strlen(ps->comm)) == 0) {
							printf("\n");
						}
						count++;
					}
					free(ps); // Remember to free up memory to prevent memory leakage
				}
			}
		}
		// Close the directory
		closedir (dir);
	} else {
		printf("error: could not read the proc directory\n");
	}
	return count;
}

void printProcStat(ProcStat *stat) {
	// Print the pid, cmd, rtime, utime and stime of the statistic
	// if the statistic is not empty
	if (stat != NULL) {
		char *realTimeString, *userTimeString, *systemTimeString;
		// Realtime is calculated by (the current uptime of the system * clock tick per second) - (process start time)
		realTimeString = timeToString(getUptime() * (double) sysconf(_SC_CLK_TCK) - stat->starttime);
		userTimeString = timeToString(stat->utime);
		systemTimeString = timeToString(stat->stime);
		printf("\n");
		printf("%s\t%-15s\t%-10s\t%-10s\t%-10s\n", "PID", "CMD", "RTIME", "UTIME", "STIME");
		printf("%d\t%-15s\t%-10s\t%-10s\t%-10s\n", (int) stat->pid, stat->comm, realTimeString, userTimeString, systemTimeString);
		// The strings are allocated by malloc, so need to free up to prevent memory leakage
		free(realTimeString);
		free(userTimeString);
		free(systemTimeString);
	}
}

void printViewTree() {
	// Print the viewtree with starting process "myshell"
	ProcStat *ps = getProcStat(getpid());
	printf("%s", ps->comm);
	if (searchChild(getpid(), strlen(ps->comm)) == 0) {
		printf("\n");
	}
}
