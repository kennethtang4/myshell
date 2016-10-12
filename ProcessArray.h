#include "Process.h"

typedef struct ProcessArray {
	Process* proc;
	unsigned length, maxLength;
} ProcessArray;

void ProcessArray_init(ProcessArray* processArray);
void ProcessArray_update(ProcessArray* processArray);
void ProcessArray_destruct(ProcessArray* processArray);
