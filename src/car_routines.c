#include "car.h"

void CarsCsvToBin(FILE* csv, char* binFileName)
{
    // open the binary file
    FILE* bin = fopen(binFileName, "wb+");
    
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

void printAllCars()
{
    // open the files
    FILE* bin = fopen("tests/veiculo5.bin", "r"); // TEST

    int n = getTotalNumberRegisters(bin);
    Car* car = newCar();

    CarHeader* header = newCarHeader();
    getCarHeader(header, bin, BIN);
    
    for (int i = 0; i < n; i++)
    {
        readCar(car, bin, BIN);
        if(printCar(car, header))
            printf("\n");
    }

    freeCarHeader(header);
    freeCar(car);
    fclose(bin);
}