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
    char binFileName[MAX_STRING_SIZE];
    scanf("%s", binFileName);
    FILE* bin = fopen(binFileName, "rb");

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

void printSelectedCars()
{
    char binFileName[MAX_STRING_SIZE], fieldName[MAX_STRING_SIZE];
    scanf("%s %s", binFileName, fieldName);

    FILE* bin = fopen(binFileName, "rb");

    CarHeader* header = newCarHeader();
    getCarHeader(header, bin, BIN);
    int nRegisters = getTotalNumberRegisters(bin);

    // Checking which field to use, and which value to search for.
    CarField field = getCarField(fieldName);
    CarSearchable search = CarSearchUsing(field);
    Car* car = newCar();

    // Print lines that match our search.
    while(nRegisters--) {
        readCar(car, bin, BIN);
        // Checking whether line matches, then printing it if it isn't logically removed
        if (checkIfCarMatches(car, field, search))
        {
            printCar(car, header);
            printf("\n");
        }
    }

    // Freeing data that will not be used anymore
    freeCarHeader(header);
    freeCar(car);
    fclose(bin);
}