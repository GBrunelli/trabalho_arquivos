#ifndef LINE_H
#define LINE_H

#include "project.h"
#include "utils.h"

// Enum of all searchable struct fields inside a line
typedef enum _LineField { COD_LINHA, ACEITA_CARTAO, NOME_LINHA, COR_LINHA } LineField;

// Union used in conjunction with LineField. Is used to search inside a struct, 
// but without compile-time knowledge of which field to use
typedef union _LineSearchable {
    int32_t codLinha;
    char aceitaCartao;
    char nomeLinha[MAX_STRING_SIZE];
    char corLinha[MAX_STRING_SIZE];
} LineSearchable;


typedef struct _LineHeader LineHeader;

typedef struct _Line Line;

/* ## Functions to deal with line headers ## */

// Create a new line header
LineHeader* newLineHeader();

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
void updateLineHeader(LineHeader* lh, FILE* file, Source from);

// Check whether LineHeader status is '1' or '0'
bool checkFileIntegrity(LineHeader* lh);

// Overwrite old LineHeader from file with a newer, currently in-memory one.
void overwriteLineHeader(LineHeader* lh, FILE* file, Source source);

// Returns how many non logically removed registers there are.
int getNRegisters(LineHeader* lh);

// Returns how many logically removed registers there are
int getNRemovedRegisters(LineHeader* lh);

// Free all data associated with a line register
void freeLineHeader(LineHeader* lh);

// Set a binary line file as INCONSISTENT or CONSISTENT, depending on supplied C
void setLineFileStatus(FILE *file, char c);

/* ## Basic line functions ## */

// Creates a new reusable Line.
Line* newLine();

// Free all memory associated with a line
void freeLine(Line* l);

// Prints Line. Also checks if Line is logically removed and deals with nulls.
FuncStatus printLine(Line* l, LineHeader* lh);


// Updates a Line with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
FuncStatus updateLine(Line* l, FILE* file, Source from);

// Writes a Line to a specific source
// Currently only supports BIN files.
FuncStatus writeLine(Line* l, FILE* file, Source from);

/* ## Functions related to searching using a specific struct field ## */

// Check which field should we use to search
LineField checkField(char* str) ;

// Based on correct field, treats stding searchable value and puts it on correct union field
LineSearchable searchUsing(LineField lf);

// Based on a specific field and a specific searched value, check whether the current line matches 
bool checkIfLineMatches(Line* l, LineField field, LineSearchable search);

#endif