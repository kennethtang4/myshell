/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <stdlib.h>
#include <sys/types.h>

#include "PidArray.h"

void PidArray_init(PidArray* pidArray) {
	// Initialize a pid array by resetting everything to default
	pidArray->pids = NULL;
	pidArray->length = 0;
	pidArray->maxLength = 5;
	PidArray_update(pidArray);
}

void PidArray_insert(PidArray* pidArray, pid_t pid) {
	// Insert a pid into the pid array
	// If the array is out of space,
	// the array will add extra 5 spaces and update the memory allocation
	if (pidArray->length >= pidArray->maxLength) {
		pidArray->maxLength += 5;
		PidArray_update(pidArray);
	}
	pidArray->pids[pidArray->length] = pid;
	pidArray->length++;
}

void PidArray_delete(PidArray* pidArray, int index) {
	// Delete a pid from the pid array with is index
	// If the index is not valid, then return
	if (index < 0 || index >= pidArray->length) {
		return;
	}
	// Shifting the array to delete the specific pid
	int i;
	for (i = index; i < pidArray->length - 1; i++) {
		pidArray->pids[i] = pidArray->pids[i + 1];
	}
	pidArray->length--;
	// If the length of pid array is 10 spaces less than the max length of the array,
	// the array will reduces 5 spaces to release memory
	if (pidArray->length + 10 < pidArray->maxLength) {
		pidArray->maxLength -= 5;
		PidArray_update(pidArray);
	}
}

int PidArray_indexOf(PidArray* pidArray, pid_t pid) {
	// Find the index of the pid in the pid array.
	// If none found, -1 is return instead.
	int i;
	for (i = 0; i < pidArray->length; i++) {
		if (pidArray->pids[i] == pid) {
			return i;
		}
	}
	return -1;
}

void PidArray_update(PidArray* pidArray) {
	// Reallocate pid array memory
	pidArray->pids = realloc(pidArray->pids, sizeof(pid_t) * pidArray->maxLength);
}

void PidArray_destruct(PidArray* pidArray) {
	// Free up the memory of the pid array and reset all values
	free(pidArray->pids);
	pidArray->length = 0;
	pidArray->maxLength = 0;
}
