#include <stdlib.h>

#include "ProcessArray.h"

void ProcessArray_init(ProcessArray* processArray) {
	processArray->proc = NULL;
	processArray->length = 0;
	processArray->maxLength = 5;
	ProcessArray_update(processArray);
}

void ProcessArray_update(ProcessArray* processArray) {
	processArray->proc = realloc(processArray->proc, sizeof(Process)*processArray->maxLength);
}

void ProcessArray_destruct(ProcessArray* processArray) {
	free(processArray->proc);
	processArray->length = 0;
	processArray->maxLength = 0;
}
