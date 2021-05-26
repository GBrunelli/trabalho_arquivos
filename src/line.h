#ifndef LINE_H
#define LINE_H

#include "project.h"
#include "utils.h"

// Use current file position instead of a specific offset.
#define CURRENT_POSITION -11

typedef enum _LineField { COD_LINHA, ACEITA_CARTAO, NOME_LINHA, COR_LINHA } LineField;

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

// Generates a LineHeader from a valid binary file. 
void _updateLineHeaderFromBin(LineHeader* lh, FILE* file);

// Generates a LineHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
void _updateLineHeaderFromCSV(LineHeader* lh, FILE* file);

// Overwrite old LineHeader from file with a newer, currently in-memory one.
void overwriteLineHeader(LineHeader* lh, FILE* file, Source source);

int getNRegisters(LineHeader* lh);
int getNRemovedRegisters(LineHeader* lh);

void freeLineHeader(LineHeader* lh);

void setLineFileStatus(FILE *file, char c);

/* ## Basic line functions ## */

// Creates a new reusable Line.
Line* newLine();

// Prints Line. Checks if Line is logically removed and also deals with nulls.
FuncStatus printLine(Line* l, LineHeader* lh);

// Free all memory associated with a line
void freeLine(Line* l);

/* ## Functions related to updating lines from different sources. ## */

// Updates a Line with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
FuncStatus updateLine(Line* l, FILE* file, Source from);

// Update Line from BIN file using fromByte as offset.
// If fromByte == CURRENT_POSITION. Ignore any offset and uses current position
FuncStatus _updateLineFromBin(Line* l, FILE* bin);

// Update Line from CSV File. Consumes current line in file buffer
FuncStatus _updateLineFromCSVLine(Line* l, FILE* csv);

// Update Line from Command Line. Consumes current stdin buffer
void _updateLineFromCLI(Line* l);

/* ## Functions related to writing lines to different sources ## */

// Writes a Line to a specific source
// Currently only supports BIN files.
FuncStatus writeLine(Line* l, FILE* file, Source from);

// Writes Line to end of binary file.
FuncStatus _writeLineToBin(Line* l, FILE* bin);

/* ## Functions related to searching using a specific struct field ## */

LineField checkField(char* str) ;

LineSearchable searchUsing(LineField lf);

bool checkIfLineMatches(Line* l, LineField field, LineSearchable search);

#endif