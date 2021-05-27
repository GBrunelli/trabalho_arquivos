#ifndef CAR_H
#define CAR_H

#include "project.h"
#include "utils.h"

typedef enum _CarField { PREFIXO, DATA, QTD_LUGARES, COD_LINHA_CAR, MODELO, CATEGORIA } CarField;

typedef struct _CarHeader CarHeader;

typedef struct _Car Car;

typedef union _CarSearchable {
    char prefixo[5];
    char data[10];
    int32_t quantidadeLugares;
    char modelo[MAX_STRING_SIZE];
    char categoria[MAX_STRING_SIZE];
} CarSearchable;

/* ## Functions to deal with Car headers ## */

// Gets the sum of active and removed register in the bin file
int getTotalNumberRegisters(FILE* file);

// Creates a new reusable CarHeader.
CarHeader* newCarHeader();

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
CarHeader* getCarHeader(CarHeader* carHeader, FILE* file, Source from);

// Set the status of a file as consistent '1' or inconsistent '0'
void setFileStatus(FILE *file, char c);

/* ## Basic Car functions ## */

// Creates a new reusable Car.
Car* newCar();

// Reads a car at the current file pointer from a source file. For bin files, if
// the pointer is pointing at the header, it will read the first car in the file.
Car* readCar(Car *c, FILE* file, Source From);

// Prints Car. Checks if Car is logically removed and also deals with nulls.
int printCar(Car* car, CarHeader* header);

// Free all memory associated with a Car
void freeCar(Car* c);

// Free all memory associated with a CarHeader
void freeCarHeader(CarHeader* carHeader);

/* ## Functions related to updating Cars from different sources. ## */

// Updates a Car with data from a specific source. 
// Currently supported sources: BIN, CLI, CSV
// If updating from CLI, file should be NULL.
void updateCar(Car* c, FILE* file, Source from);

/* ## Functions related to writing Cars to different sources ## */

// Writes a Car to a specific source
// Currently only supports BIN files.
void writeCar(Car* c, FILE* file, Source from);

void writeCarHeader(CarHeader* carHeader, FILE* file, Source from);

/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search, 
// Returns CarField representation of struct field found 
CarField getCarField(char* providedField);

char* getCarContent(Car* car, CarField field);

char* getHeaderDescription(CarHeader* header, CarField field);

CarSearchable CarSearchUsing(CarField field);

bool checkIfCarMatches(Car* car, CarField field, CarSearchable search);

#endif