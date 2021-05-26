#include "line_routines.h"
#include "line.h"

void LinesCsvToBin() {
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
    getLineHeader(lh, csvFile, CSV);
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