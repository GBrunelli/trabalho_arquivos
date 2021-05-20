#include "car.h"
#include "line.h"

void CarsCsvToBin(FILE* file, char* binFileName)
{
    // open the binary file
    FILE* bin = fopen(binFileName, "r+b");
    
    // read the header from the csv
    CarHeader* carHeader = getCarHeader(file, CSV);

    // while there are cars in the csv

        // read the car from the csv
        // write the car in the bin

    // write the header in the bin

    // free the memory
}