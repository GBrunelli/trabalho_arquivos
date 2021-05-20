#include "project.h"
#include "routines.h"

struct _CarHeader {
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

// Generates a CarHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
CarHeader* _getCarHeaderFromCSV(FILE* file)
{

}

// Overwrite old CarHeader from file with a newer, currently in-memory one.
void overwriteCarHeader(CarHeader ch, FILE* file)
{

}


/* ## Basic Car functions ## */

// Creates a new reusable Car.
Car* newCar()
{

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