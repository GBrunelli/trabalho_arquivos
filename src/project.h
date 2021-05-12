#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Maximum field size for each struct.
#define MAX_STRING_SIZE 100

// Possible file sources.
typedef enum _Source { CSV, CLI, BIN } Source;

#endif