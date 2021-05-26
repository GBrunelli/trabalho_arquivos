#include "line.h"
#include "utils.h"

#define LINE_HEADER_OFFSET 82
#define LINE_OFFSET 13

#define DESCREVECODIGO_SIZE 15
#define DESCREVECARTAO_SIZE 13
#define DESCREVENOME_SIZE 13
#define DESCREVECOR_SIZE 24
struct _Line {
    int8_t removido;
    int32_t tamanhoRegistro;
    int32_t codLinha;
    int8_t aceitaCartao;
    int32_t tamanhoNome;
    char nomeLinha[MAX_STRING_SIZE];
    int32_t tamanhoCor;
    char nomeCor[MAX_STRING_SIZE];
};

struct _LineHeader {
    int8_t status;
    int64_t byteProxReg;
    int32_t nroRegistros;
    int32_t nroRegistrosRemovidos;
    char descreveCodigo[DESCREVECODIGO_SIZE];
    char descreveCartao[DESCREVECARTAO_SIZE];
    char descreveNome[DESCREVENOME_SIZE];
    char descreveCor[DESCREVECOR_SIZE];
};


LineHeader* newLineHeader()
{
    LineHeader* lh = calloc(1, sizeof(LineHeader));
    lh->status = 0;
    lh->nroRegistros = 0;
    lh->nroRegistrosRemovidos = 0;
    lh->byteProxReg = LINE_HEADER_OFFSET;
    return lh;
}

void _getLineHeaderFromBin(LineHeader* lh, FILE* file) {
    fseek(file, 0, SEEK_SET);

    fread(&lh->status, sizeof(lh->status), 1, file);
    fread(&lh->byteProxReg, sizeof(lh->byteProxReg), 1, file);
    fread(&lh->nroRegistros, sizeof(lh->nroRegistros), 1, file);
    fread(&lh->nroRegistrosRemovidos, sizeof(lh->nroRegistrosRemovidos), 1, file);
    fread(&lh->descreveCodigo, sizeof(lh->descreveCodigo), 1, file);
    fread(&lh->descreveCartao, sizeof(lh->descreveCartao), 1, file);
    fread(&lh->descreveNome, sizeof(lh->descreveNome), 1, file);
    fread(&lh->descreveCor, sizeof(lh->descreveCor), 1, file);
}

void _getLineHeaderFromCSV(LineHeader* lh, FILE* file) {
    // set the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // read the line
    char *buffer = calloc(1, 256);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%[^\n]%*c", buffer);

    // get each column
    char* token = strsep(&buffer, ",");
    strncpy(lh->descreveCodigo, token, DESCREVECODIGO_SIZE);

    token = strsep(&buffer, ",");
    strncpy(lh->descreveCartao, token, DESCREVECARTAO_SIZE);

    token = strsep(&buffer, ",");
    strncpy(lh->descreveNome, token, DESCREVENOME_SIZE);

    token = strsep(&buffer, ",");
    strncpy(lh->descreveCor, token, DESCREVECOR_SIZE);

    free(buffer_pointer);
}

void getLineHeader(LineHeader* LineHeader, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            _getLineHeaderFromCSV(LineHeader, file);
            break;
        case BIN:
            _getLineHeaderFromBin(LineHeader, file);
            break;
        default:
            break;
    }
}

void _overwriteLineHeaderToBin(LineHeader* lh, FILE* file)
{
    // set the file pointer to the correct position
    fseek(file, 0, SEEK_SET);

    // write the data on the file
    fwrite(&lh->status, sizeof(lh->status), 1, file);
    fwrite(&lh->byteProxReg, sizeof(lh->byteProxReg), 1, file);
    fwrite(&lh->nroRegistros, sizeof(lh->nroRegistros), 1, file);
    fwrite(&lh->nroRegistrosRemovidos, sizeof(lh->nroRegistrosRemovidos), 1, file);
    fwrite(&lh->descreveCodigo, sizeof(lh->descreveCodigo), 1, file);
    fwrite(&lh->descreveCartao, sizeof(lh->descreveCartao), 1, file);
    fwrite(&lh->descreveNome, sizeof(lh->descreveNome), 1, file);
    fwrite(&lh->descreveCor, sizeof(lh->descreveCor), 1, file);  
}


void overwriteLineHeader(LineHeader* lh, FILE* file, Source source) {
    switch (source)
    {
    case BIN:
        _overwriteLineHeaderToBin(lh, file);
        break;

    default:
        break;
    }
}

// Set the status of a file as consistent '1' or inconsistent '0'
void setLineFileStatus(FILE *file, char c)
{
    if(c == REMOVED || c == NOT_REMOVED)
    {
        LineHeader* lh = newLineHeader();
        getLineHeader(lh, file, BIN);
        lh->status = c;
        overwriteLineHeader(lh, file, BIN);
        freeLineHeader(lh);
    }
}

/* ## Basic Line functions ## */
Line* newLine() {
    Line* l = calloc(1, sizeof(Line));
    l->removido = '1';
    l->tamanhoRegistro = LINE_OFFSET;
    return l;
}

FuncStatus _updateLineFromCSVLine(Line* l, FILE* file) {
    if (l == NULL) return UNKNOWN_ERR;
    if (file == NULL) return UNKNOWN_ERR;

    // Reading the line
    char codLinha[5] = {0};
    char aceitaCartao[2] = {0};
    char nomeLinha[MAX_STRING_SIZE] = {0};
    char corLinha[MAX_STRING_SIZE] = {0};
    
    if (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]%*c", codLinha, aceitaCartao, nomeLinha, corLinha) != 4)
        return EOF_OR_MALFORMED;

    // Logically removed Lines start with *. Checking whether current line is logically removed.
    if (codLinha[0] == '*') {
        l->removido = REMOVED;
        l->codLinha = atoi(&codLinha[1]);
    } else {
        l->removido = NOT_REMOVED;
        l->codLinha = atoi(codLinha);
    }

    // Setting fixed size parameters
    l->aceitaCartao = aceitaCartao[0];

    // Setting variable sized parameters and their sizes
    if (isNULO(nomeLinha))
        nomeLinha[0] = '\0';
    if (isNULO(corLinha))
        corLinha[0] = '\0';
    
    l->tamanhoNome = strlen(nomeLinha);
    l->tamanhoCor = strlen(corLinha);
    l->tamanhoRegistro = LINE_OFFSET + l->tamanhoCor + l->tamanhoNome;
    strcpy(l->nomeLinha, nomeLinha);
    strcpy(l->nomeCor, corLinha);

    return OK;
};

FuncStatus updateLine(Line* l, FILE* file, Source from) {
    switch (from)
    {
    case CSV:
        return _updateLineFromCSVLine(l, file);
        break;
    default:
        break;
    }
    return UNKNOWN_ERR;
}

FuncStatus _writeLineToBin(Line* l, FILE* file) {
    // gets the information from the header and update it
    LineHeader* lh = newLineHeader();
    getLineHeader(lh, file, BIN);

    int byteOffset = lh->byteProxReg;
    lh->byteProxReg += l->tamanhoRegistro + sizeof(l->removido) + sizeof(l->tamanhoRegistro);

    if (l->removido == NOT_REMOVED)
        lh->nroRegistros++;
    else
        lh->nroRegistrosRemovidos++;

    overwriteLineHeader(lh, file, BIN);
    freeLineHeader(lh);

    fseek(file, byteOffset, SEEK_SET);
    fwrite(&l->removido, sizeof(l->removido), 1, file);
    fwrite(&l->tamanhoRegistro, sizeof(l->tamanhoRegistro), 1, file);
    fwrite(&l->codLinha, sizeof(l->codLinha), 1, file);
    fwrite(&l->aceitaCartao, sizeof(l->aceitaCartao), 1, file);
    fwrite(&l->tamanhoNome, sizeof(l->tamanhoNome), 1, file);
    fwrite(&l->nomeLinha, l->tamanhoNome, 1, file);
    fwrite(&l->tamanhoCor, sizeof(l->tamanhoCor), 1, file);
    fwrite(&l->nomeCor, l->tamanhoCor, 1, file);

    return OK;
}

FuncStatus writeLine(Line* l, FILE* file, Source from) {
    switch (from)
    {
    case BIN:
        return _writeLineToBin(l, file);
        break;
    
    default:
        break;
    }

    return UNKNOWN_ERR;
}

// Prints Car. Checks if Car is logically removed and also deals with nulls.
void printLine(Line* l) {

}


void freeLine(Line* l) {
    free(l);
}

void freeLineHeader(LineHeader* lh) {
    free(lh);
}

