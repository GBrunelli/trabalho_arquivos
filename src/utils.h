#ifndef UTILS_H
#define UTILS_H

#include "project.h"

// Message used when printing fields that are currently Null
#define NULL_MESSAGE "campo com valor nulo"

// Maximum field size for each struct.
#define MAX_STRING_SIZE 100

// Logically removed
#define REMOVED '0'
// Not logically removed
#define NOT_REMOVED '1'

// Inconsistent file
#define STATUS_INCONSISTENT '0'
// Consistent file
#define STATUS_CONSISTENT '1'

// Possible file sources.
typedef enum _Source
{
    CSV,
    CLI,
    BIN
} Source;

// Possible results from functions.
// Can be: Errors, Incomplete Results, OK, etc...
typedef enum _FuncStatus
{
    UNKNOWN_ERR,
    OK,
    EOF_OR_MALFORMED,
    NO_FILE_FOUND,
    LOGICALLY_REMOVED
} FuncStatus;

// ## Utility functions ## //

// Shift N chars to the left
void leftShift(char *string, int len);

// Check if string equals "NULO"
int isNULO(char *string);

// Format garbage filled string using "\0@@@@" format
void fillWithGarbage(char *string, int len);

// Remove "Quotation Marks" from a string.
void removeQuotations(char *str);

// ## Supplied functions on funcao_fornecida.c ## //

void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif
