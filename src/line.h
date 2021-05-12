#ifndef LINE_H
#define LINE_H

#include "project.h"

// Use current file position instead of a specific offset.
#define CURRENT_POSITION -11

typedef enum _LineField { COD_LINHA, ACEITA_CARTAO, LINHA, COR } LineField;

typedef struct _LineHeader {
    int8_t status;
    int64_t byteProxReg;
    int32_t nroRegistros;
    int32_t nroRegistrosRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveCor[24];
} LineHeader;

typedef struct _Line {
    int8_t removido;
    int32_t tamanhoRegistro;
    int32_t codLinha;
    int8_t aceitaCartao;
    int32_t tamanhoNome;
    char nomeLinha[MAX_STRING_SIZE];
    int32_t tamanhoCor;
    char nomeCor[MAX_STRING_SIZE];
} Line;

/* ## Functions to deal with line headers ## */

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
LineHeader getLineHeader(FILE* file, Source from);

// Generates a LineHeader from a valid binary file. 
LineHeader _getLineHeaderFromBin(FILE* file);

// Generates a LineHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
LineHeader _getLineHeaderFromCSV(FILE* file);

// Overwrite old LineHeader from file with a newer, currently in-memory one.
void overwriteLineHeader(LineHeader lh, FILE* file);

/* ## Basic line functions ## */

// Creates a new reusable Line.
Line* newLine();

// Prints Line. Checks if Line is logically removed and also deals with nulls.
void printLine(Line* l);

// Free all memory associated with a line
void freeMemLine(Line* l);

/* ## Functions related to updating lines from different sources. ## */

// Updates a Line with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
void updateLine(Line* l, FILE* file, Source from);

// Update Line from BIN file using fromByte as offset.
// If fromByte == CURRENT_POSITION. Ignore any offset and uses current position
void _updateLineFromBin(Line* l, FILE* bin, int64_t fromByte);

// Update Line from CSV File. Consumes current line in file buffer
void _updateLineFromCSVLine(Line* l, FILE* csv);

// Update Line from Command Line. Consumes current stdin buffer
void _updateLineFromCLI(Line* l);

/* ## Functions related to writing lines to different sources ## */

// Writes a Line to a specific source
// Currently only supports BIN files.
void writeLine(Line* l, FILE* file, Source from);

// Writes Line to end of binary file.
void _writeLineToBin(Line* l, FILE* bin);

/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search, 
// Returns LineField representation of struct field found 
LineField getLineField(LineHeader lh, char* providedField);

// Returns true if a line's field == searched value
bool checkLineByField(Line* l, LineField field, char value[100]);

#endif