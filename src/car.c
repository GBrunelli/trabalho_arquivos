#include "project.h"
#include "routines.h"
#include <string.h>

struct _CarHeader {
    // 175 bytes
    int8_t status;
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
    // at least 36 bytes
    int8_t removido;
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

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
CarHeader* getCarHeader(FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            return _getCarHeaderFromCSV(file);
        
        case BIN:
            return _getCarHeaderFromBin(file);
    
        default:
            return NULL;
    }
}

// Generates a CarHeader from a valid binary file. 
CarHeader* _getCarHeaderFromBin(FILE* file)
{

}

/**
 * NEED TO TEST 
 * */
// Generates a CarHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
CarHeader* _getCarHeaderFromCSV(FILE* file)
{
    CarHeader* carHeader = malloc(sizeof(CarHeader));
    carHeader->status = 0;
    carHeader->byteProxReg = 175;
    carHeader->nroRegistros = 0;
    carHeader->nroRegistrosRemovidos = 0;
    
    fseek(file, 0, SEEK_SET);

    // read the line
    char *buffer = calloc(1, MAX_STRING_SIZE);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%s", buffer);

    // get each column
    char *token = strsep(&buffer, ",");
    sscanf(token, "%s", carHeader->descrevePrefixo);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", carHeader->descreveData);           

    *token = strsep(&buffer, ",");
    sscanf(token, "%d", carHeader->descreveLugares);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", carHeader->descreveLinha);

    *token = strsep(&buffer, ",");
    sscanf(token, "%d", carHeader->descreveModelo);

    *token = strsep(&buffer, ",");
    sscanf(token, "%d", carHeader->descreveCategoria);

    free(buffer_pointer);

    return carHeader;
}

// Overwrite old CarHeader from file with a newer, currently in-memory one.
void overwriteCarHeader(CarHeader ch, FILE* file)
{

}


/* ## Basic Car functions ## */

// Creates a new reusable Car.
Car* newCar()
{
    Car* car = calloc(1, sizeof(Car));
    car->removido = 1;
    car->tamanhoRegistro = 36;
    return car;
}

// Shifts a string to left.
void leftShift(char *string, int len)
{
    int i;
    for(i = 1; i < len; i++)
    {
        string[i - 1] = string[i];
    }
    string[len -1] = '/0';
}

// Reads a car from a source file.
Car* readCar(Car* car, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            _readCarFromCSV(car, file);
            break;

        default:
            break;
    }
}

Car* _readCarFromCSV(Car *car, FILE *file)
{
    // read the line
    char *buffer = calloc(1, MAX_STRING_SIZE);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%s", buffer);

    // get each column
    char *token = strsep(&buffer, ",");
    sscanf(token, "%s", car->prefixo);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", car->data);     

    *token = strsep(&buffer, ",");
    sscanf(token, "%d", car->quantidadeLugares);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", car->codLinha);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", car->modelo);

    *token = strsep(&buffer, ",");
    sscanf(token, "%s", car->categoria);          

    // verify if the register is removed
    if(car->prefixo[0] = "*")
    {
        car->removido = 0;
        leftShift(car->prefixo, 5);
    }

    // calculates the lenght of car->modelo and car->categoria
    int lenghtModelo = strlen(car->modelo);
    int lenghtCategoria = strlen(car->categoria);
    car->tamanhoModelo = lenghtModelo;
    car->tamanhoCategoria = lenghtCategoria;

    // calculates the size of the register
    car->tamanhoRegistro += lenghtModelo + lenghtCategoria;

    free(buffer_pointer);
    return car;
}

// Prints Car. Checks if Car is logically removed and also deals with nulls.
void printCar(Car* c)
{

}

// Free all memory associated with a Car
void freeMemCar(Car* c)
{

}


/* ## Functions related to updating Cars from different sources. ## */

// Updates a Car with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
void updateCar(Car* c, FILE* file, Source from)
{

}

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


/* ## Functions related to writing Cars to different sources ## */

// Writes a Car to a specific source
// Currently only supports BIN files.
void writeCar(Car* c, FILE* file, Source from)
{

}

// Writes Car to end of binary file.
void _writeCarToBin(Car* c, FILE* bin)
{

}


/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search, 
// Returns CarField representation of struct field found 
CarField* getCarField(CarHeader ch, char* providedField)
{

}

// Returns true if a Car's field == searched value
bool checkCarByField(Car* c, CarField field, char* value)
{

}