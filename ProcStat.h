/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <sys/types.h>

// A structure that will record all informations in /proc/{pid}/stat file
typedef struct ProcStat {
	pid_t pid, ppid, pgrp;
	char comm[255];
	char state;
	int session, tty_nr, tpgid, exit_signal, processor;
	unsigned flags, rt_priority, policy;
	unsigned long minflt, cminflt, majflt, cmajflt, utime, stime, vsize, rsslim;
	unsigned long startcode, endcode, startstack, kstkesp, kstkeip, signal;
	unsigned long blocked, sigignore, sigcatch, wchan, nswap, cnswap;
	unsigned long guest_time;
	long cutime, cstime, priority, nice, num_threads, itrealvalue, rss;
	long cguest_time;
	unsigned long long starttime, delayacct_blkio_ticks;
} ProcStat;

ProcStat *getProcStat(pid_t pid); // Get the specific pid's process statistic 
void printProcStat(ProcStat *stat); // Print the process statistic out
void printViewTree(); // Print the tree created by viewtree command
