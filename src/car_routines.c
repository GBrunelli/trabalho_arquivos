#include "car.h"

void CarsCsvToBin()
{
    // open the binary file

    FILE* csv = fopen("tests/veiculo1.csv", "r"); // TEST
    FILE* bin = fopen("veiculo1.bin", "wb+");
    
    // read the header from the csv and write in the bin
    CarHeader* carHeader = newCarHeader();
    getCarHeader(carHeader, csv, CSV);
    writeCarHeader(carHeader, bin, BIN);

    // set the file status as inconsistent 
    setFileStatus(bin, STATUS_INCONSISTENT);

    // instanciate a car holder
    Car* car = newCar();
    Car* result;

    result = readCar(car, csv, CSV);
    while(result != NULL) // while there are cars in the csv
    {
        writeCar(car, bin, BIN); // write the car in the bin 
        result = readCar(car, csv, CSV); // read the car from the csv
    }

    // set the file status as consistent 
    setFileStatus(bin, STATUS_CONSISTENT);

    // free the heap used in this funtion 
    fclose(bin);
    freeCarHeader(carHeader);
    freeCar(car);
}