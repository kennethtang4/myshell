/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <sys/types.h>

typedef struct PidArray {
	pid_t* pids;
	unsigned length, maxLength;
} PidArray;

void PidArray_init(PidArray* pidArray);
void PidArray_insert(PidArray* pidArray, pid_t pid);
void PidArray_delete(PidArray* pidArray, int index);
int PidArray_indexOf(PidArray* pidArray, pid_t pid);
void PidArray_update(PidArray* pidArray);
void PidArray_destruct(PidArray* pidArray);
