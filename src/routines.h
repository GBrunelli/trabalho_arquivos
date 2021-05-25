#ifndef ROUTINES_H
#define ROUTINES_H

#include "car.h"

/* ## CAR ROUTINES ## */

// 1th task -> INSERT ALL
void CarsCsvToBin(FILE* file, char* binFileName);

// 3nd task -> SELECT ALL
void printAllCars();

// 5rd task -> SELECT WHERE Campo = Valor
void printSelectedCars();

// 7th task -> INSERT N REGISTERS INTO END
void InsertNewCarsBin();


#endif