/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
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
void sigUsr1Action();

static PidArray timeXPids, foregroundPids;
static int ready = 0, in = 0, saved_stdout, fd[2];

int main() {
	// Register signal handlers
	signal(SIGINT, ctrlCAction);
	signal(SIGCHLD, childProcExit);
	signal(SIGUSR1, sigUsr1Action);
	// Initialize the array contains pid for timeX
	PidArray_init(&timeXPids);
	PidArray_init(&foregroundPids);
	while (1) {
		// Ask for user input
		printf("## myshell $ ");
		char *input = getInput(10);
		// Check if the user wants to exit the shell
		// Extra spacing after exit is allowed but extra arguments are not allowed
		if (strncmp(input, "exit", 4) == 0) {
			int i = 4;
			if (input[i] == ' ') {
				while (input[i++] == ' ');
			}
			if (input[i] == '\0') {
				free(input);
				printf("myshell: Terminated\n");
				break;
			} else {
				printf("myshell: \"exit\" with other arguments!!!\n");
				free(input);
				continue;
			}
		}
		// Check if user enter command "viewtree"
		// Extra spacing is allowed
		if (strncmp(input, "viewtree", 8) == 0) {
			int i = 8;
			if (input[i] == ' ') {
				while (input[i++] == ' ');
			}
			if (input[i] == '\0') {
				printViewTree();
			} else {
				printf("myshell: \"viewtree\" with other arguments!!!\n");
			}
			free(input);
			continue;
		}
		// Check if the command is a background command
		int background = isBackground(input);
		if (background == -1) {
			printf("myshell: '&' should not appear in the middle of the command line\n");
			free(input);
			continue;
		}
		// Check if the command is a timeX command
		int timeX = isTimeX(input);
		// Prohibit timeX to run in background mode
		if (background == 1 && timeX == 1) {
			printf("myshell: \"timeX\" cannot be run in background mode\n");
			free(input);
			continue;
		}
		// If there is no command then go back to the start of the loop
		if (input[0] == '\0') {
			free(input);
			continue;
		}

		Process process;
		Process_init(&process, input, background, timeX); // Initialize the process

		if (process.length == 0) {
			continue;
		}

		newProcess(&process); // Create new process

		Process_destruct(&process); // Destruct the process to free up memory and prevent memory leakage

		free(input); // Free the input to prevent memory leakage
	}
	// Free the timeX pids array and foregournd pids array
	// to prevent memory leakage
	// (though should not happen in modern operating system
	// as it is end of the process)
	PidArray_destruct(&timeXPids);	PidArray_destruct(&foregroundPids);
	return 0;
}

void newProcess(Process* process) {
	// Set the ready state for execution to false
	ready = 0;
	// Create new child process
	int i;
	in = 0;
	for (i = 0; i < process->length; i++) {
		pipe(fd);
		pid_t pid = fork();
		if (pid < 0) {
			printf("fork: error no = %s\n", strerror(errno));
			exit(-1);
		} else if (pid == 0) {
			if (process->background == 1) {
				setpgid(0, 0);
			}
			if (in != 0) {
				dup2(in, 0);
			}
			if (i != process->length - 1) {
				if (fd[1] != 1) {
					saved_stdout = dup(1);
					dup2(fd[1], 1);
				}
			}
			// Execute the command
			execute(process->commands[i]);
		} else {
			if (process->timeX == 1) {
				// If the process is a timeX command,
				// insert the pid into the timeXPids array for later output
				PidArray_insert(&timeXPids, pid);
			}
			// Send the self defined signal to the child process to make its ready state to be true
			kill(pid, SIGUSR1);
			if (process->background == 0) {
				// If the process is not background process than wait until it finishes
				PidArray_insert(&foregroundPids, pid);
				if (i == process->length - 1) {
					while (foregroundPids.length > 0) {
						siginfo_t infop;
						waitid(P_PID, foregroundPids.pids[0], &infop, WNOWAIT | WEXITED);
					}
				}
			}
		}
		close(fd[1]);
		in = fd[0];
	}
}

void execute(char* command) {
	// Keep checking if the command is ready for execution
	while (ready == 0) {
		usleep(10);
	}
	// Get the arguments of the command
	char** arg = parseExec(command);
	// Execute the program
	if (execvp(arg[0], arg) == -1) {
		dup2(saved_stdout, 1);
		close(saved_stdout);
		printf("myshell: '%s': %s\n", arg[0], strerror(errno));
		exit(-1);
	}
	exit(0);
}

void ctrlCAction() {
	// Ignore the ctrl + c action
	if (foregroundPids.length == 0) {
		printf("\n## myshell $ ");
		fflush(stdout);
	}
}

void childProcExit() {
	// One or more of the child processes have exited
	siginfo_t infop;
	int waitidResult;
	int status;
	// Get the id with WNOHANG and WNOWAIT options
	// This allow foreground process working expectedly
	// And also not to terminate the zombie process in order to get the statistic later on
	while ((waitidResult = waitid(P_ALL, 0, &infop, WNOHANG | WNOWAIT | WEXITED)) >= 0) {
		// Get the pid of the terminiated process
		pid_t pid = infop.si_pid;
		if (pid == 0) {
			break;
		}
		// Check if the terminated pid is a process of timeX
		int timeXPidIndex = PidArray_indexOf(&timeXPids, pid);
		if (timeXPidIndex != -1) {
			// If the process is a timeX process then output the statistic and delete the pid from timeXPids array
			ProcStat *stat = getProcStat(pid);
			printProcStat(stat);
			PidArray_delete(&timeXPids, timeXPidIndex);
		}
		// Check if the teriminated pid is a foreground process
		int foregroundPidIndex = PidArray_indexOf(&foregroundPids, pid);
		if (foregroundPidIndex == -1) {
			// If the it is not a foregroud process than output a message that the process has been terminated
			ProcStat *stat = getProcStat(pid);
			if (stat != NULL) {
				printf("[%d] %s Done\n", (int) stat->pid, stat->comm);
				free(stat);
			} else {
				printf("[%d] Done\n", (int) pid);
			}
		} else {
			PidArray_delete(&foregroundPids, foregroundPidIndex);
		}
		// Clean up the zombie process
		waitpid(pid, &status, 0);
	}
}

void sigUsr1Action() {
	// Set the ready state to true
	ready = 1;
}
