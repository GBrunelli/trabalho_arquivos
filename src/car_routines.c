#include "car.h"

void CarsCsvToBin()
{
    char csvFileName[MAX_STRING_SIZE], binFileName[MAX_STRING_SIZE];
    scanf("%s %s", csvFileName, binFileName);

    // open the binary file
    FILE* csv = fopen(csvFileName, "r");
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
    
    binarioNaTela(binFileName);
}

void printAllCars()
{
    // open the files
    FILE* bin = fopen("tests/veiculo5.bin", "r"); // TEST
    
    
}