#ifndef UTILS_H
#define UTILS_H

#include "project.h"

// Message used when printing fields that are currently Null
#define NULL_MESSAGE = "campo com valor nulo"

// Maximum field size for each struct.
#define MAX_STRING_SIZE 100

#define REMOVED '0'
#define NOT_REMOVED '1'

#define STATUS_INCONSISTENT '0'
#define STATUS_CONSISTENT '1'

// Possible file sources.
typedef enum _Source { CSV, CLI, BIN } Source;

// Possible types of data
typedef enum _DataType { CAR, LINE } DataType;

// Results from functions.
// Can be: Errors, Incomplete Results, OK, etc...
typedef enum _FuncStatus { UNKNOWN_ERR, OK } FuncStatus;



// Our utils
void leftShift(char *string, int len);
int isNULO(char* string);
void fillWithGarbage(char* string, int len);

// Supplied functions
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif
