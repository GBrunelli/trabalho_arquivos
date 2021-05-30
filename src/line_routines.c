#include "line_routines.h"
#include "line.h"

// 2th task -> INSERT ALL
void LinesCsvToBin(void)
{
    // Getting CSV and BIN filenames
    char csvFileName[MAX_STRING_SIZE], binFileName[MAX_STRING_SIZE];
    scanf("%s %s", csvFileName, binFileName);

    // Opening both src and dest files. Then checking whether they are valid
    FILE *csvFile = fopen(csvFileName, "r");
    if (csvFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    FILE *binFile = fopen(binFileName, "wb+");
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(csvFile);
        exit(0);
    }

    // Writing LineHeader from CSV into Binary file
    LineHeader *lh = newLineHeader();
    updateLineHeader(lh, csvFile, CSV);
    overwriteLineHeader(lh, binFile, BIN);
    freeLineHeader(lh);

    // Setting file as INCONSISTENT
    setLineFileStatus(binFile, STATUS_INCONSISTENT);

    // Instantiating a Line and then writing all CSV Lines into BIN file
    Line *l = newLine();
    while (updateLine(l, csvFile, CSV) == OK)
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

// 4th task -> SELECT ALL
void printAllLines(void)
{
    // Getting filename from stdin
    char binFileName[MAX_STRING_SIZE];
    scanf("%s", binFileName);

    // Opening file and dealing with errors
    FILE *bin = fopen(binFileName, "rb");
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    LineHeader *lh = newLineHeader();
    updateLineHeader(lh, bin, BIN);
    if (!checkFileIntegrity(lh))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        freeLineHeader(lh);
        exit(0);
    }

    // Checking how many registers there are
    int nRegisters = getNRegisters(lh);
    int nRemovedRegisters = getNRemovedRegisters(lh);

    // If there are no registers, stop the processing and return "Registro inexistente."
    if (nRegisters == 0)
    {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeLineHeader(lh);
        exit(0);
    }

    Line *l = newLine();
    nRegisters += nRemovedRegisters;

    // Print all registers that aren't logically removed
    while (nRegisters--)
    {
        if (updateLine(l, bin, BIN) != OK)
        {
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

// 6th task -> SELECT WHERE Campo = Valor
void printSelectedLines(void)
{
    // Scanning from stdin, then opening files
    char binFileName[MAX_STRING_SIZE], fieldName[MAX_STRING_SIZE];
    scanf("%s %s", binFileName, fieldName);

    FILE *bin = fopen(binFileName, "rb");
    if (bin == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Checking file integrity
    LineHeader *lh = newLineHeader();
    updateLineHeader(lh, bin, BIN);
    if (!checkFileIntegrity(lh))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        freeLineHeader(lh);
        exit(0);
    }

    // Checking how many registers there are
    int nRegisters = getNRegisters(lh);
    int nRemovedRegisters = getNRemovedRegisters(lh);

    // If there are no registers, stop the processing and return "Registro inexistente."
    if (nRegisters == 0)
    {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeLineHeader(lh);
        exit(0);
    }
    nRegisters += nRemovedRegisters;

    // Checking which field to use, and which value to search for.
    LineField field = checkField(fieldName);
    LineSearchable search = searchUsing(field);
    Line *l = newLine();

    // Print lines that match our search.
    int n_matches = 0;      // Number of matches
    FuncStatus status = OK; // Used to check whether a line is removed or not
    while (nRegisters--)
    {
        if (updateLine(l, bin, BIN) != OK)
        {
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

// 8th task -> INSERT N REGISTERS INTO END
void InsertNewLinesBin(void)
{
    // Getting stdin data for number of new registers and filename
    int nRegisters = 0;
    char binFileName[MAX_STRING_SIZE];
    scanf("%s %d", binFileName, &nRegisters);

    // Opening both src and dest files
    FILE *binFile = fopen(binFileName, "r+b");

    // Error handling if file didn't open correctly
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Checking file integrity
    LineHeader *lh = newLineHeader();
    updateLineHeader(lh, binFile, BIN);
    if (!checkFileIntegrity(lh))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(binFile);
        freeLineHeader(lh);
        exit(0);
    }
    freeLineHeader(lh);

    // Setting file as INCONSISTENT
    setLineFileStatus(binFile, STATUS_INCONSISTENT);

    // Instantiating a Line and then writing all CSV Lines into BIN file
    fseek(binFile, 0, SEEK_END);
    Line *l = newLine();
    while (nRegisters--)
    {
        updateLine(l, NULL, CLI);
        writeLine(l, binFile, BIN);
    }
    freeLine(l);

    // Finishing program. Freeing all memory
    setLineFileStatus(binFile, STATUS_CONSISTENT);
    fclose(binFile);

    // Proof that our program worked correctly
    binarioNaTela(binFileName);
}