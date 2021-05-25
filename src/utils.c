#include "utils.h"

// Shifts a string to left.
void leftShift(char *string, int len)
{
    int i;
    for(i = 1; i < len; i++)
    {
        string[i - 1] = string[i];
    }
    string[len - 1] = 0;
}

int isNULO(char* string)
{
    if(!strcmp(string, "NULO"))
    {
        return 1;
    }
    return 0;
}

void fillWithGarbage(char* string, int len)
{
    string[0] = 0;
    for (int i = 1; i < len; i++)
    {
        string[i] = '@';
    }
}