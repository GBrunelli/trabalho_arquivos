#include "utils.h"

// Remove "Quotation Marks" from a string.
void removeQuotations(char *str)
{
    // Checking whether string has quotation marks
    if (str[0] != '"')
        return;

    int len = strlen(str);
    for (int i = 1; i < len; i++)
    {
        str[i - 1] = str[i];
    }

    str[len - 2] = '\0';
}

// Shifts a string to left.
void leftShift(char *string, int len)
{
    int i;
    for (i = 1; i < len; i++)
    {
        string[i - 1] = string[i];
    }
    string[len - 1] = 0;
}

int isNULO(char *string)
{
    if (!strcmp(string, "NULO"))
    {
        return 1;
    }
    return 0;
}

void fillWithGarbage(char *string, int len)
{
    string[0] = 0;
    for (int i = 1; i < len; i++)
    {
        string[i] = '@';
    }
}