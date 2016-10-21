/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

int isBackground(char *input); // Check if the command should run in background
int isTimeX(char *input); // Check if it is a timeX command
char **parseInput(char *input); // Split the command into different parts for piping
char **parseExec(char *exec); // Split the command into different parts of arguments for execvp
