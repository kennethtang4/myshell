/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

#include <sys/types.h>

// A structure that defines an array of pids
// Length and the maxLength of the array is recorded
// and therefore can be accessed quickly
typedef struct PidArray {
	pid_t* pids;
	unsigned length, maxLength;
} PidArray;

void PidArray_init(PidArray* pidArray); // Initialize a pid array
void PidArray_insert(PidArray* pidArray, pid_t pid); // Insert a pid into the pid array
void PidArray_delete(PidArray* pidArray, int index); // Delete a pid with its index from the pid array
int PidArray_indexOf(PidArray* pidArray, pid_t pid); // Get index of a pid from the pid array
void PidArray_update(PidArray* pidArray); // Update the pid array memoery allocation
void PidArray_destruct(PidArray* pidArray); // Destroy the pid array by freeing its components memory
