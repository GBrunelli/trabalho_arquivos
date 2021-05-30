#ifndef CAR_H
#define CAR_H

#include "project.h"
#include "utils.h"

// Enum of all searchable struct fields inside a Car
typedef enum _CarField
{
    PREFIXO,
    DATA,
    QTD_LUGARES,
    COD_LINHA_CAR,
    MODELO,
    CATEGORIA
} CarField;

// Union used in conjunction with CarField. Is used to search inside a struct,
// but without compile-time knowledge of which field to use
typedef union _CarSearchable
{
    char prefixo[5];
    char data[10];
    int32_t quantidadeLugares;
    char modelo[MAX_STRING_SIZE];
    char categoria[MAX_STRING_SIZE];
} CarSearchable;

typedef struct _CarHeader CarHeader;

typedef struct _Car Car;

/* ## Functions to deal with Car headers ## */

// Gets the sum of active and removed register in the bin file
int getTotalNumberRegisters(FILE *file);

// Alocates memory and initializes the struct CarHeader
CarHeader *newCarHeader();

// Get all Header information from a specific source file.
// Currently supported sources: BIN, CSV
CarHeader *getCarHeader(CarHeader *carHeader, FILE *file, Source from);

// Set the status of a file as consistent '1' or inconsistent '0'
void setFileStatus(FILE *file, char c);

// Verify if the file is consistent.
// Returns 0 if it is inconsistent, or a value
// different than zero if it is consistent
int checkCarFileIntegrity(CarHeader *header);

/* ## Basic Car functions ## */

// Alocates memory and initializes the struct Car
Car *newCar();

// Reads a car at the current file pointer from a source file. For bin files, if
// the pointer is pointing at the header, it will read the first car in the file.
Car *readCar(Car *c, FILE *file, Source From);

// Prints Car. Checks if Car is logically removed and also deals with nulls.
int printCar(Car *car, CarHeader *header);

// Free all memory associated with a Car.
void freeCar(Car *c);

// Free all memory associated with a CarHeader.
void freeCarHeader(CarHeader *carHeader);

/* ## Functions related to writing Cars to different sources ## */

// Writes a Car to a specific source
// Currently only supports BIN files.
void writeCar(Car *c, FILE *file, Source from);

// Writes a CarHeader to a specific source
// Currently only supports BIN files.
void writeCarHeader(CarHeader *carHeader, FILE *file, Source from);

/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search,
// Returns CarField representation of struct field found
CarField getCarField(char *providedField);

// Get a new string with value of the field of a Car.
// The string must be freed by the user.
char *getCarContent(Car *car, CarField field);

// Get a new string with the field description of a determined field of a CarHeader.
// The string must be freed by the user.
char *getHeaderDescription(CarHeader *header, CarField field);

// Returns a CarSearchable union given a CarField.
CarSearchable CarSearchUsing(CarField field);

// Check if the the car match the search.
// Returns true if it is a match, false if it isn't.
bool checkIfCarMatches(Car *car, CarField field, CarSearchable search);

#endif