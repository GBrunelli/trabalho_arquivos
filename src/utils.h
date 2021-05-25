#ifndef UTILS_H
#define UTILS_H

#include "project.h"

// Message used when printing fields that are currently Null
#define NULL_MESSAGE = "campo com valor nulo"

// Maximum field size for each struct.
#define MAX_STRING_SIZE 100

#define REMOVED '0'
#define NOT_REMOVED '1'

// Possible file sources.
typedef enum _Source { CSV, CLI, BIN } Source;

typedef enum _DataType { CAR, LINE } DataType;


void leftShift(char *string, int len);
int isNULO(char* string);
void fillWithGarbage(char* string, int len);


#endif
