#include "car.h"
#include "utils.h"

#define STRUCT_CAR_HEADER_SIZE 175
#define STRUCT_BASE_CAR_SIZE 31 // ! Não seria 32? Já que vai de 0...31

struct _CarHeader {
    char status;
    int64_t byteProxReg;
    int32_t nroRegistros;
    int32_t nroRegistrosRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];
};

struct _Car {
    char removido;
    int32_t tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int32_t quantidadeLugares;
    int32_t codLinha;
    int32_t tamanhoModelo;
    char modelo[MAX_STRING_SIZE];
    int32_t tamanhoCategoria;
    char categoria[MAX_STRING_SIZE];
};

/* ## Functions to deal with Car headers ## */

// Alocates memory and initializes the struct
CarHeader* newCarHeader()
{
    CarHeader* carHeader = calloc(1, sizeof(CarHeader));
    carHeader->byteProxReg = STRUCT_CAR_HEADER_SIZE;
    return carHeader;
}

// Generates a CarHeader from a valid binary file.
// ! Não precisa retornar em nenhuma dessas funções
// ! Passou como ponteiro ele já altera no próprio fread
CarHeader* _getCarHeaderFromBin(CarHeader* carHeader, FILE* file)
{
    fseek(file, 0, SEEK_SET);
    fread(&carHeader->status,                   sizeof(carHeader->status),                  1, file);
    fread(&carHeader->byteProxReg,              sizeof(carHeader->byteProxReg),             1, file);
    fread(&carHeader->nroRegistros,             sizeof(carHeader->nroRegistros),            1, file);
    fread(&carHeader->nroRegistrosRemovidos,    sizeof(carHeader->nroRegistrosRemovidos),   1, file);
    fread(&carHeader->descrevePrefixo,          sizeof(carHeader->descrevePrefixo),         1, file);
    fread(&carHeader->descreveData,             sizeof(carHeader->descreveData),            1, file);
    fread(&carHeader->descreveLugares,          sizeof(carHeader->descreveLugares),         1, file);
    fread(&carHeader->descreveLinha,            sizeof(carHeader->descreveLinha),           1, file);
    fread(&carHeader->descreveModelo,           sizeof(carHeader->descreveModelo),          1, file);
    fread(&carHeader->descreveCategoria,        sizeof(carHeader->descreveCategoria),       1, file);
    return carHeader;
}

// Generates a CarHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
CarHeader* _getCarHeaderFromCSV(CarHeader* carHeader, FILE* file)
{    
    // set the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // read the line
    char *buffer = calloc(1, 256);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%[^\n]%*c", buffer);

    // get each column
    char* token = strsep(&buffer, ",");
    strcpy(carHeader->descrevePrefixo, token);

    token = strsep(&buffer, ",");
    strcpy(carHeader->descreveData, token);           

    token = strsep(&buffer, ",");
    strcpy(carHeader->descreveLugares, token);

    token = strsep(&buffer, ",");
    strcpy(carHeader->descreveLinha, token);

    token = strsep(&buffer, ",");
    strcpy(carHeader->descreveModelo, token);

    token = strsep(&buffer, ",");
    strcpy(carHeader->descreveCategoria, token);

    free(buffer_pointer);

    return carHeader;
}

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
CarHeader* getCarHeader(CarHeader* carHeader, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            return _getCarHeaderFromCSV(carHeader, file);
        
        case BIN:
            return _getCarHeaderFromBin(carHeader, file);
    
        default:
            break;
    }
    return NULL;
}

// Write the CarHeader in a bin file
void _writeCarHeaderToBin(CarHeader* carHeader, FILE* file)
{
    // set the file pointer to the correct position
    fseek(file, 0, SEEK_SET);

    // write the data on the file
    fwrite(&carHeader->status,                  sizeof(carHeader->status),                  1, file);
    fwrite(&carHeader->byteProxReg,             sizeof(carHeader->byteProxReg),             1, file);
    fwrite(&carHeader->nroRegistros,            sizeof(carHeader->nroRegistros),            1, file);
    fwrite(&carHeader->nroRegistrosRemovidos,   sizeof(carHeader->nroRegistrosRemovidos),   1, file);
    fwrite(&carHeader->descrevePrefixo,         sizeof(carHeader->descrevePrefixo),         1, file);
    fwrite(&carHeader->descreveData,            sizeof(carHeader->descreveData),            1, file);
    fwrite(&carHeader->descreveLugares,         sizeof(carHeader->descreveLugares),         1, file);
    fwrite(&carHeader->descreveLinha,           sizeof(carHeader->descreveLinha),           1, file);
    fwrite(&carHeader->descreveModelo,          sizeof(carHeader->descreveModelo),          1, file);
    fwrite(&carHeader->descreveCategoria,       sizeof(carHeader->descreveCategoria),       1, file);    
}

// Writes a CarHeader to a specific source
// Currently only supports BIN files.
void writeCarHeader(CarHeader* carHeader, FILE* file, Source from)
{
    switch (from)
    {
        case BIN:
            _writeCarHeaderToBin(carHeader, file);
            break;

        default:
            break;
    }
}

// Set the status of a file as consistent '1' or inconsistent '0'
void setFileStatus(FILE *file, char c)
{
    if(c == REMOVED || c == NOT_REMOVED)
    {
        CarHeader* header = newCarHeader();
        header = getCarHeader(header, file, BIN);
        header->status = c;
        writeCarHeader(header, file, BIN);
        freeCarHeader(header);
    }
}



/* ## Basic Car functions ## */

// Creates a new reusable Car.
Car* newCar()
{
    Car* car = malloc(sizeof(struct _Car));
    car->removido = NOT_REMOVED;
    car->tamanhoRegistro = STRUCT_BASE_CAR_SIZE;
    return car;
}

Car* _readCarFromCSV(Car *car, FILE *file)
{
    if(car == NULL) return NULL;
    if(file == NULL) return NULL;

    // read the line
    char *buffer = calloc(1, 256);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%[^\n]%*c", buffer);

    if(*buffer != 0)
    {
        // get each column
        char *token = strsep(&buffer, ",");
        strcpy(car->prefixo, token);

        // DATA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->data, sizeof(car->data));
        else
            strcpy(car->data, token);     
        
        // QUANTIDADE DE LUGARES
        token = strsep(&buffer, ",");
        if(isNULO(token))
            car->quantidadeLugares = -1;
        else
            sscanf(token, "%d", &car->quantidadeLugares);

        // COD LINHA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            car->codLinha = -1;
        else
            sscanf(token, "%d", &car->codLinha);

        // MODELO
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->modelo, sizeof(car->modelo));
        else
            strcpy(car->modelo, token);    

        // CATEGORIA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->categoria, sizeof(car->categoria));
        else
            strcpy(car->categoria, token);         

        // verify if the register is removed
        if(car->prefixo[0] == '*')
        {
            car->removido = REMOVED;
            leftShift(car->prefixo, 5);
        }
        else 
            car->removido = NOT_REMOVED;

        // calculates the lenght of car->modelo and car->categoria
        int lenghtModelo = strlen(car->modelo);
        int lenghtCategoria = strlen(car->categoria);
        car->tamanhoModelo = lenghtModelo;
        car->tamanhoCategoria = lenghtCategoria;

        // calculates the size of the register
        car->tamanhoRegistro = STRUCT_BASE_CAR_SIZE + lenghtModelo + lenghtCategoria;
    }
    else
    {
        free(buffer_pointer);
        return NULL;
    }

    free(buffer_pointer);
    return car;
}

// Reads a car from a source file.
Car* readCar(Car* car, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            return _readCarFromCSV(car, file);

        default:
            break;
    }
    return NULL;
}

// Prints Car. Checks if Car is logically removed and also deals with nulls.
void printCar(Car* c)
{

}

// Free all memory associated with a Car
void freeCar(Car* c)
{
    free(c);
}

// Free all memory associated with a CarHeader
void freeCarHeader(CarHeader* carHeader)
{
    free(carHeader);
}




/* ## Functions related to updating Cars from different sources. ## */

// Update Car from BIN file using fromByte as offset.
// If fromByte == CURRENT_POSITION. Ignore any offset and uses current position
void _updateCarFromBin(Car* c, FILE* bin, int64_t fromByte)
{

}

// Update Car from CSV File. Consumes current Car in file buffer
void _updateCarFromCSVCar(Car* c, FILE* csv)
{

}

// Update Car from Command Car. Consumes current stdin buffer
void _updateCarFromCLI(Car* c)
{

}

// Updates a Car with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
void updateCar(Car* c, FILE* file, Source from)
{

}




/* ## Functions related to writing Cars to different sources ## */

// Writes Car to end of binary file.
void _writeCarToBin(Car* car, FILE* file)
{
    // gets the information from the header and update it
    CarHeader* header = newCarHeader();
    getCarHeader(header, file, BIN);
    header->status = '0';
    int byteOffset = header->byteProxReg;
    header->byteProxReg += car->tamanhoRegistro + sizeof(car->removido) + sizeof(car->tamanhoRegistro);

    // verifica se o registro é removido ou não
    if(car->removido == NOT_REMOVED)
    {
        header->nroRegistros++;
    }
    else
    {
        header->nroRegistrosRemovidos++;
    }

    writeCarHeader(header, file, BIN);
    freeCarHeader(header);
    
    // set the file pointer to the correct position
    fseek(file, byteOffset, SEEK_SET);

    // write the car in the file
    fwrite(&car->removido,           sizeof(car->removido),          1, file);
    fwrite(&car->tamanhoRegistro,    sizeof(car->tamanhoRegistro),   1, file);
    fwrite(&car->prefixo,            sizeof(car->prefixo),           1, file);
    fwrite(&car->data,               sizeof(car->data),              1, file);
    fwrite(&car->quantidadeLugares,  sizeof(car->quantidadeLugares), 1, file);
    fwrite(&car->codLinha,           sizeof(car->codLinha),          1, file);
    fwrite(&car->tamanhoModelo,      sizeof(car->tamanhoModelo),     1, file);
    fwrite(&car->modelo,             car->tamanhoModelo,             1, file);
    fwrite(&car->tamanhoCategoria,   sizeof(car->tamanhoCategoria),  1, file);
    fwrite(&car->categoria,          car->tamanhoCategoria,          1, file);
}

// Writes a Car to a specific source
// Currently only supports BIN files.
void writeCar(Car* car, FILE* file, Source from)
{
    switch (from)
    {
        case BIN:
            _writeCarToBin(car, file);
            break;

        default:
            break;
    }
}




/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search, 
// Returns CarField representation of struct field found 
CarField* getCarField(CarHeader ch, char* providedField)
{
    return NULL;
}

// Returns true if a Car's field == searched value
bool checkCarByField(Car* c, CarField field, char* value)
{
    return NULL;
}
