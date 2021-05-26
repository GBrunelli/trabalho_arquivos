// #include "car_routines.h"
#include "line_routines.h"
#include "car_routines.h"

int main(void) {
    // Routine to be executed
    int n_routine = 0;
    scanf("%d", &n_routine);
    

    switch (n_routine) {
    case 1:
        CarsCsvToBin();
        break;
    case 2:
        LinesCsvToBin();
        break;
    case 3:
        printAllCars();
        break;
    case 4:
        printAllLines();
        break;
    case 5:
        //printSelectedCars();
        break;
    case 6:
        printSelectedLines();
        break;
    case 7:
        //InsertNewCarsBin();
        break;
    case 8:
        InsertNewLinesBin();
        break;
    default:
        break;
    }

    return EXIT_SUCCESS;
}