#include "routines.h"

int main(void) {
    // Routine to be executed
    int8_t n_routine = 1;
    //scanf("%d", &n_routine);

    // TODO: read input
    
    FILE* csvFile = fopen("tests/veiculo1.csv", "r"); // TEST

    switch (n_routine) {
    case 1:
        CarsCsvToBin(csvFile, "veiculo1.bin"); // TEST
        break;
    case 2:
        //LinesCsvToBin();
        break;
    case 3:
        //printAllCars();
        break;
    case 4:
        //printAllLines();
        break;
    case 5:
        //printSelectedCars();
        break;
    case 6:
        //printSelectedLines();
        break;
    case 7:
        //InsertNewCarsBin();
        break;
    case 8:
        //InsertNewLinesBin();
        break;
    default:
        break;
    }
}