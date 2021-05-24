#include "car.h"
#include "line.h"

void CarsCsvToBin(FILE* file, char* binFileName)
{
    // open the binary file
    FILE* bin = fopen(binFileName, "r+b");
    
    // read the header from the csv
    CarHeader* carHeader = getCarHeader(file, CSV);

    // write the header
    writeCarHeader(carHeader, file, BIN);

    // instanciate a car holder
    Car* car = newCar();

    readCar(car, file, CSV);
    while(car != NULL) // while there are cars in the csv
    {
        writeCar(car, bin, BIN); // read the car from the csv
        readCar(car, file, CSV); // write the car in the bin
    }

    // free the memory
}