#include "line_routines.h"
#include "line.h"

void LinesCsvToBin() {
    char csvFileName[MAX_STRING_SIZE], binFileName[MAX_STRING_SIZE];
    scanf("%s %s", csvFileName, binFileName);

    // Opening both src and dest files
    FILE* csvFile = fopen(csvFileName, "r");
    FILE* binFile = fopen(binFileName, "wb+");
    
    // Writing LineHeader from CSV into Binary file
    LineHeader* lh = newLineHeader();
    getLineHeader(lh, csvFile, CSV);
    overwriteLineHeader(lh, binFile, BIN);
    freeLineHeader(lh);

    // Setting file as INCONSISTENT
    setLineFileStatus(binFile, STATUS_INCONSISTENT);
    
    // Instantiating a car holder
    Line* l = newLine();
    while(updateLine(l, csvFile, CSV) == OK) // while there are cars in the csv
    {
        writeLine(l, binFile, BIN); // write the car in the bin 
    }
    freeLine(l);
    fclose(csvFile);
    
    setLineFileStatus(binFile, STATUS_CONSISTENT);
    fclose(binFile);
    
    binarioNaTela(binFileName);
}