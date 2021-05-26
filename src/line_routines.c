#include "line_routines.h"
#include "line.h"

void LinesCsvToBin(void) {
    // Getting CSV and BIN filenames
    char csvFileName[MAX_STRING_SIZE], binFileName[MAX_STRING_SIZE];
    scanf("%s %s", csvFileName, binFileName);

    // Opening both src and dest files
    FILE* csvFile = fopen(csvFileName, "r");
    FILE* binFile = fopen(binFileName, "wb+");

    // Error handling didn't open
    if (csvFile == NULL || binFile == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(csvFile);
        fclose(binFile);

        exit(1);
    }
    
    // Writing LineHeader from CSV into Binary file
    LineHeader* lh = newLineHeader();
    updateLineHeader(lh, csvFile, CSV);
    overwriteLineHeader(lh, binFile, BIN);
    freeLineHeader(lh);

    // Setting file as INCONSISTENT
    setLineFileStatus(binFile, STATUS_INCONSISTENT);
    
    // Instantiating a Line and then writing all CSV Lines into BIN file
    Line* l = newLine();
    while(updateLine(l, csvFile, CSV) == OK) 
    {
        writeLine(l, binFile, BIN);  
    }

    // Finishing program. Freeing all memory
    freeLine(l);
    fclose(csvFile);
    setLineFileStatus(binFile, STATUS_CONSISTENT);
    fclose(binFile);
    
    // Proof that our program worked correctly
    binarioNaTela(binFileName);
}

void printAllLines(void) {
    // Getting filename from stdin
    char binFileName[MAX_STRING_SIZE];
    scanf("%s", binFileName);

    // Opening file and dealing with errors
    FILE* bin = fopen(binFileName, "rb");
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        exit(0);
    }

    LineHeader* lh = newLineHeader();
    updateLineHeader(lh, bin, BIN);

    // Checking how many registers there are
    int nRegisters = getNRegisters(lh);
    int nRemovedRegisters = getNRemovedRegisters(lh);

    // If there are no registers, stop the processing and return "Registro inexistente."
    if (nRegisters == 0) {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeLineHeader(lh);

        exit(0);
    }

    Line* l = newLine();
    nRegisters += nRemovedRegisters;

    // Print all registers that aren't logically removed
    while(nRegisters--) {
        if (updateLine(l, bin, BIN) != OK) {
            printf("Falha no processamento do arquivo.\n");
                freeLineHeader(lh);
                freeLine(l);
                fclose(bin);
                exit(0);
        }

        printLine(l, lh);
    }

    // Finishing program. Freeing all memory
    freeLineHeader(lh);
    freeLine(l);
    fclose(bin);
}

void printSelectedLines(void) {
    char binFileName[MAX_STRING_SIZE], fieldName[MAX_STRING_SIZE];
    scanf("%s %s", binFileName, fieldName);

    FILE* bin = fopen(binFileName, "rb");
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        exit(0);
    }

    // Checking how many registers there are
    LineHeader* lh = newLineHeader();
    updateLineHeader(lh, bin, BIN);
    int nRegisters = getNRegisters(lh);
    int nRemovedRegisters = getNRemovedRegisters(lh);

    // If there are no registers, stop the processing and return "Registro inexistente."
    if (nRegisters == 0) {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeLineHeader(lh);
        exit(0);
    }
    nRegisters += nRemovedRegisters;


    // Checking which field to use, and which value to search for.
    LineField field = checkField(fieldName);
    LineSearchable search = searchUsing(field);
    Line* l = newLine();
    int n_matches = 0; // Number of matches
    FuncStatus status; // Used to check whether a line is removed or not

    // Print lines that match our search.
    while(nRegisters--) {
        if (updateLine(l, bin, BIN) != OK) {
            printf("Falha no processamento do arquivo.\n");
                freeLineHeader(lh);
                freeLine(l);
                fclose(bin);
                exit(0);
        }

        // Checking whether line matches, then printing it if it isn't logically removed
        if (checkIfLineMatches(l, field, search))
            status = printLine(l, lh);
        
        if (status != LOGICALLY_REMOVED)
            n_matches++;
    }

    // Check whether matches were found
    if (n_matches == 0)
        printf("Registro inexistente.\n");

    // Freeing data that will not be used anymore
    freeLineHeader(lh);
    freeLine(l);
    fclose(bin);
}