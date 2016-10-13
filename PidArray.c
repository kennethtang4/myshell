#include <stdlib.h>
#include <sys/types.h>

#include "PidArray.h"

void PidArray_init(PidArray* pidArray) {
	pidArray->pids = NULL;
	pidArray->length = 0;
	pidArray->maxLength = 5;
	PidArray_update(pidArray);
}

void PidArray_insert(PidArray* pidArray, pid_t pid) {
	if (pidArray->length >= pidArray->maxLength) {
		pidArray->maxLength += 5;
		PidArray_update(pidArray);
	}
	pidArray->pids[pidArray->length] = pid;
	pidArray->length++;
}

void PidArray_delete(PidArray* pidArray, int index) {
	if (index < 0 || index >= pidArray->length) {
		return;
	}
	int i;
	for (i = index; i < pidArray->length - 1; i++) {
		pidArray->pids[i] = pidArray->pids[i + 1];
	}
	pidArray->length--;
	if (pidArray->length < pidArray->maxLength - 10) {
		pidArray->maxLength -= 5;
		PidArray_update(pidArray);
	}
}

int PidArray_indexOf(PidArray* pidArray, pid_t pid) {
	int i;
	for (i = 0; i < pidArray->length; i++) {
		if (pidArray->pids[i] == pid) {
			return i;
		}
	}
	return -1;
}

void PidArray_update(PidArray* pidArray) {
	pidArray->pids = realloc(pidArray->pids, sizeof(pid_t) * pidArray->maxLength);
}

void PidArray_destruct(PidArray* pidArray) {
	free(pidArray->pids);
	pidArray->length = 0;
	pidArray->maxLength = 0;
}
