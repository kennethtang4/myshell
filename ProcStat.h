#include <sys/types.h>

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