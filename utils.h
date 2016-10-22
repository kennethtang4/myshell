/*
 * Student name and no: Kenneth Tang (3035123459)
 * Student name and no:
 * Development platform: Ubuntu 14.04 LTS
 * Last modified date: 21/10/2016
 * Compilation: make (details in Makefile)
 */

void StringArray_destruct(char** stringArray); // Destruct a string array
int sizeOfDynamic(char *input); // Get the length of a string
int isStringEmpty(char *input); // Check if string is empty, string contains space and tab also considered as empty
int isStringLastCharWithSpace(char *input, char ch); // Check if string is end with a character while space and tab are excluded
char *timeToString (unsigned long time); // Convert unsigned long time ticks to 2 decimal string
