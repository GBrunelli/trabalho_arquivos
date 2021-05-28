#include "car.h"

void CarsCsvToBin()
{
    char csvFileName[MAX_STRING_SIZE], binFileName[MAX_STRING_SIZE];
    scanf("%s %s", csvFileName, binFileName);

    // Open the csv, if an error occur prints "Falha no processamento do arquivo."
    FILE* csv = fopen(csvFileName, "r");
    if (csv == NULL) 
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Creates the binary file, if an error occur prints "Falha no processamento do arquivo."
    FILE* bin = fopen(binFileName, "wb+");
    if (bin == NULL) 
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Read the header from the csv and write in the bin
    CarHeader* carHeader = newCarHeader();
    getCarHeader(carHeader, csv, CSV);
    writeCarHeader(carHeader, bin, BIN);

    // Set the file status as inconsistent 
    setFileStatus(bin, STATUS_INCONSISTENT);

    // Instanciate a car holder
    Car* car = newCar();
    Car* result;

    // read the first car
    result = readCar(car, csv, CSV);
    while(result != NULL) // while there are cars in the csv
    {
        writeCar(car, bin, BIN); // write the car in the bin 
        result = readCar(car, csv, CSV); // read the car from the csv
    }

    // Set the file status as consistent 
    setFileStatus(bin, STATUS_CONSISTENT);

    // Free the heap used in this funtion 
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
    if (bin == NULL) 
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    CarHeader* header = newCarHeader();
    getCarHeader(header, bin, BIN);
    if(!checkCarFileIntegrity(header))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        freeCarHeader(header);
        exit(0);
    }

    int nRegisters = getTotalNumberRegisters(bin);
    // If there are no registers, stop the processing and return "Registro inexistente."
    if (nRegisters == 0) {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeCarHeader(header);
        exit(0);
    }
    
    Car* car = newCar();
    while (nRegisters--)
    {
        readCar(car, bin, BIN);
        if(printCar(car, header))
        {
            printf("\n");
        }   
    }

    freeCarHeader(header);
    freeCar(car);
    fclose(bin);
}

void printSelectedCars()
{
    // Read the input and open the file
    char binFileName[MAX_STRING_SIZE], fieldName[MAX_STRING_SIZE];
    scanf("%s %s", binFileName, fieldName);
    FILE* bin = fopen(binFileName, "rb");
    if (bin == NULL) 
    {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Checking file consistency
    CarHeader* header = newCarHeader();
    getCarHeader(header, bin, BIN);
    if(!checkCarFileIntegrity(header))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        freeCarHeader(header);
        exit(0);
    }

    //If there are no registers, stop the processing and print "Registro inexistente."
    int nRegisters = getTotalNumberRegisters(bin);
    if (nRegisters == 0) 
    {
        printf("Registro inexistente.\n");
        fclose(bin);
        freeCarHeader(header);
        exit(0);
    }

    // Checking which field to use, and which value to search for.
    CarField field = getCarField(fieldName);
    CarSearchable search = CarSearchUsing(field);
    Car* car = newCar();

    // Print lines that match our search.
    char match = 0;
    while(nRegisters--) {
        readCar(car, bin, BIN);
        // Checking whether line matches, then printing it if it isn't logically removed
        if (checkIfCarMatches(car, field, search))
        {
            printCar(car, header);
            printf("\n");
            match = 1;
        }
    }

    // Check whether matches were found
    if (!match)
        printf("Registro inexistente.\n");

    // Freeing data that will not be used anymore
    freeCarHeader(header);
    freeCar(car);
    fclose(bin);
}

void InsertNewCarsBin()
{
    // Getting stdin data for number of new registers and filename
    int nRegisters = 0;
    char binFileName[MAX_STRING_SIZE];
    scanf("%s %d", binFileName, &nRegisters);

    // Opening both src and dest files
    FILE* bin = fopen(binFileName, "r+b");

    // Error handling if file didn't open correctly
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // Checking file integrity
    CarHeader* header = newCarHeader();
    getCarHeader(header, bin, BIN);
    if (!checkCarFileIntegrity(header)) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        freeCarHeader(header);
        exit(0);
    }
    freeCarHeader(header);
    
    // Setting file as INCONSISTENT
    setFileStatus(bin, STATUS_INCONSISTENT);

    // Instantiating a Line and then writing all CSV Lines into BIN file
    Car* car = newCar();
    while(nRegisters--) {
        readCar(car, NULL, CLI);
        writeCar(car, bin, BIN);  
    }
    freeCar(car);

    // Finishing program. Freeing all memory
    setFileStatus(bin, STATUS_CONSISTENT);
    fclose(bin);
    
    // Proof that our program worked correctly
    binarioNaTela(binFileName);
}