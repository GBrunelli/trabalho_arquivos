#ifndef CAR_H
#define CAR_H

#include "project.h"

typedef enum _CarField { PREFIXO, DATA, QTD_LUGARES, COD_LINHA_CAR, MODELO, CATEGORIA } CarField;

typedef struct _CarHeader CarHeader;

typedef struct _Car Car;

/* ## Functions to deal with Car headers ## */

// Creates a new reusable CarHeader.
CarHeader* newCarHeader();

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
CarHeader* getCarHeader(CarHeader* carHeader, FILE* file, Source from);

// Overwrite old CarHeader from file with a newer, currently in-memory one.
void overwriteCarHeader(CarHeader ch, FILE* file);

/* ## Basic Car functions ## */


// Creates a new reusable Car.
Car* newCar();

// Reads a car from a source file.
Car* readCar(Car *c, FILE* file, Source From);

// Prints Car. Checks if Car is logically removed and also deals with nulls.
void printCar(Car* c);

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
CarField* getCarField(CarHeader ch, char* providedField);

// Returns true if a Car's field == searched value
bool checkCarByField(Car* c, CarField field, char value[100]);

#endif