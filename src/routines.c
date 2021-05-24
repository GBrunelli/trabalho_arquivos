#include "car.h"
#include "line.h"

void CarsCsvToBin(FILE* file, char* binFileName)
{
    // open the binary file
    FILE* bin = fopen(binFileName, "wb");
    
    // read the header from the csv and write in the bin
    CarHeader* carHeader = newCarHeader();
    getCarHeader(carHeader, file, CSV);
    writeCarHeader(carHeader, bin, BIN);

    // instanciate a car holder
    Car* car = newCar();

    readCar(car, file, CSV);
    while(car != NULL) // while there are cars in the csv
    {
        writeCar(car, bin, BIN); // read the car from the csv
        readCar(car, file, CSV); // write the car in the bin
    }

    // free the heap used in this funtion 
    fclose(bin);
    freeCarHeader(carHeader);
    freeCar(car);
}