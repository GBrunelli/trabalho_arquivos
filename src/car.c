#include "car.h"
#include "utils.h"

#define STRUCT_CAR_HEADER_SIZE 175
#define STRUCT_BASE_CAR_SIZE 31

struct _CarHeader {
    char status;
    int64_t byteProxReg;
    int32_t nroRegistros;
    int32_t nroRegistrosRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];
};

struct _Car {
    char removido;
    int32_t tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int32_t quantidadeLugares;
    int32_t codLinha;
    int32_t tamanhoModelo;
    char modelo[MAX_STRING_SIZE];
    int32_t tamanhoCategoria;
    char categoria[MAX_STRING_SIZE];
};

/* ## Functions to deal with Car headers ## */

// Gets the sum of active and removed register in the bin file
int getTotalNumberRegisters(FILE* file)
{
    CarHeader* header = newCarHeader();
    getCarHeader(header, file, BIN);
    int n = header->nroRegistros + header->nroRegistrosRemovidos;
    freeCarHeader(header);
    return n;
}

// Alocates memory and initializes the struct CarHeader
CarHeader* newCarHeader()
{
    CarHeader* carHeader = calloc(1, sizeof(CarHeader));
    carHeader->byteProxReg = STRUCT_CAR_HEADER_SIZE;
    return carHeader;
}

// Generates a CarHeader from a valid binary file.
CarHeader* _getCarHeaderFromBin(CarHeader* carHeader, FILE* file)
{
    // Set the file pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // Read the data
    fread(&carHeader->status,                   sizeof(carHeader->status),                  1, file);
    fread(&carHeader->byteProxReg,              sizeof(carHeader->byteProxReg),             1, file);
    fread(&carHeader->nroRegistros,             sizeof(carHeader->nroRegistros),            1, file);
    fread(&carHeader->nroRegistrosRemovidos,    sizeof(carHeader->nroRegistrosRemovidos),   1, file);
    fread(&carHeader->descrevePrefixo,          sizeof(carHeader->descrevePrefixo),         1, file);
    fread(&carHeader->descreveData,             sizeof(carHeader->descreveData),            1, file);
    fread(&carHeader->descreveLugares,          sizeof(carHeader->descreveLugares),         1, file);
    fread(&carHeader->descreveLinha,            sizeof(carHeader->descreveLinha),           1, file);
    fread(&carHeader->descreveModelo,           sizeof(carHeader->descreveModelo),          1, file);
    fread(&carHeader->descreveCategoria,        sizeof(carHeader->descreveCategoria),       1, file);
    return carHeader;
}

// Generates a CarHeader from a valid CSV file. 
// Must be first command used on CSV File or will not work.
CarHeader* _getCarHeaderFromCSV(CarHeader* carHeader, FILE* file)
{    
    // set the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // read the line
    char *buffer = calloc(1, 256);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%[^\n]%*c", buffer);

    // get each column
    char* token;
    token = strsep(&buffer, ",");
    strncpy(carHeader->descrevePrefixo,   token, sizeof(carHeader->descrevePrefixo));

    token = strsep(&buffer, ",");
    strncpy(carHeader->descreveData,      token, sizeof(carHeader->descreveData));           

    token = strsep(&buffer, ",");
    strncpy(carHeader->descreveLugares,   token, sizeof(carHeader->descreveLugares));

    token = strsep(&buffer, ",");
    strncpy(carHeader->descreveLinha,     token, sizeof(carHeader->descreveLinha));

    token = strsep(&buffer, ",");
    strncpy(carHeader->descreveModelo,    token, sizeof(carHeader->descreveModelo));

    token = strsep(&buffer, ",");
    strncpy(carHeader->descreveCategoria, token, sizeof(carHeader->descreveCategoria));

    free(buffer_pointer);

    return carHeader;
}

// Get all Header information from a specific source file. 
// Currently supported sources: BIN, CSV
CarHeader* getCarHeader(CarHeader* carHeader, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            return _getCarHeaderFromCSV(carHeader, file);
        
        case BIN:
            return _getCarHeaderFromBin(carHeader, file);
    
        default:
            break;
    }
    return NULL;
}

// Write the CarHeader in a bin file
void _writeCarHeaderToBin(CarHeader* carHeader, FILE* file)
{
    // set the file pointer to the correct position
    fseek(file, 0, SEEK_SET);

    // write the data on the file
    fwrite(&carHeader->status,                  sizeof(carHeader->status),                  1, file);
    fwrite(&carHeader->byteProxReg,             sizeof(carHeader->byteProxReg),             1, file);
    fwrite(&carHeader->nroRegistros,            sizeof(carHeader->nroRegistros),            1, file);
    fwrite(&carHeader->nroRegistrosRemovidos,   sizeof(carHeader->nroRegistrosRemovidos),   1, file);
    fwrite(&carHeader->descrevePrefixo,         sizeof(carHeader->descrevePrefixo),         1, file);
    fwrite(&carHeader->descreveData,            sizeof(carHeader->descreveData),            1, file);
    fwrite(&carHeader->descreveLugares,         sizeof(carHeader->descreveLugares),         1, file);
    fwrite(&carHeader->descreveLinha,           sizeof(carHeader->descreveLinha),           1, file);
    fwrite(&carHeader->descreveModelo,          sizeof(carHeader->descreveModelo),          1, file);
    fwrite(&carHeader->descreveCategoria,       sizeof(carHeader->descreveCategoria),       1, file);    
}

// Writes a CarHeader to a specific source
// Currently only supports BIN files.
void writeCarHeader(CarHeader* carHeader, FILE* file, Source from)
{
    switch (from)
    {
        case BIN:
            _writeCarHeaderToBin(carHeader, file);
            break;

        default:
            break;
    }
}

// Set the status of a file as consistent '1' or inconsistent '0'
void setFileStatus(FILE *file, char c)
{
    // if the char is valid
    if(c == REMOVED || c == NOT_REMOVED)
    {
        CarHeader* header = newCarHeader();
        header = getCarHeader(header, file, BIN);
        header->status = c;
        writeCarHeader(header, file, BIN);
        freeCarHeader(header);
    }
}

// Verify if the file is consistent. 
// Returns 0 if it is inconsistent, or a value 
// different than zero if it is consistent
int checkCarFileIntegrity(CarHeader* header)
{
    if(header->status == STATUS_CONSISTENT)
        return 1;
    return 0;
}

/* ## Basic Car functions ## */

// Alocates memory and initializes the struct Car
Car* newCar()
{
    Car* car = malloc(sizeof(struct _Car));
    car->removido = NOT_REMOVED;
    car->tamanhoRegistro = STRUCT_BASE_CAR_SIZE;
    return car;
}

// Reads the next car at the current file pointer
Car* _readCarFromCSV(Car *car, FILE *file)
{
    if(car == NULL) return NULL;
    if(file == NULL) return NULL;

    // read the line
    char *buffer = calloc(1, 256);
    char *buffer_pointer = buffer; // save the initial pointer location to free later
    fscanf(file, "%[^\n]%*c", buffer);

    // if the line is not null
    if(*buffer != 0)
    {
        // get each column
        char *token = strsep(&buffer, ",");
        strncpy(car->prefixo, token, sizeof(car->prefixo));

        // DATA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->data, sizeof(car->data));
        else
            strncpy(car->data, token, sizeof(car->data));     
        
        // QUANTIDADE DE LUGARES
        token = strsep(&buffer, ",");
        if(isNULO(token))
            car->quantidadeLugares = -1;
        else
            sscanf(token, "%d", &car->quantidadeLugares);

        // COD LINHA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            car->codLinha = -1;
        else
            sscanf(token, "%d", &car->codLinha);

        // MODELO
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->modelo, sizeof(car->modelo));
        else
            strncpy(car->modelo, token, sizeof(car->modelo));    

        // CATEGORIA
        token = strsep(&buffer, ",");
        if(isNULO(token))
            fillWithGarbage(car->categoria, sizeof(car->categoria));
        else
            strncpy(car->categoria, token, sizeof(car->categoria));         

        // verify if the register is removed
        if(car->prefixo[0] == '*')
        {
            car->removido = REMOVED;
            leftShift(car->prefixo, 5);
        }
        else 
            car->removido = NOT_REMOVED;

        // calculates the lenght of car->modelo and car->categoria
        int lenghtModelo = strlen(car->modelo);
        int lenghtCategoria = strlen(car->categoria);
        car->tamanhoModelo = lenghtModelo;
        car->tamanhoCategoria = lenghtCategoria;

        // calculates the size of the register
        car->tamanhoRegistro = STRUCT_BASE_CAR_SIZE + lenghtModelo + lenghtCategoria;
    }
    else
    {
        free(buffer_pointer);
        return NULL;
    }

    free(buffer_pointer);
    return car;
}

// Reads the next car at the current file pointer
Car* _readCarFromBIN(Car *car, FILE *file)
{
    // if the pointer is pointing at the header, set the pointer for the first car in the file
    long long position = ftell(file);
    long long offset = position < STRUCT_CAR_HEADER_SIZE ? STRUCT_CAR_HEADER_SIZE : position;
    fseek(file, offset, SEEK_SET);

    // reads the field "removido" and "tamanhoRegistro" to analyse if car is valid
    fread(&car->removido,            sizeof(car->removido),          1, file);
    fread(&car->tamanhoRegistro,     sizeof(car->tamanhoRegistro),   1, file);
    // if the car is logically removed, point to the next car
    if(car->removido == REMOVED) 
    {
        fseek(file, car->tamanhoRegistro, SEEK_CUR); 
        return NULL;
    }
    // read the data
    fread(&car->prefixo,             sizeof(car->prefixo),           1, file);
    fread(&car->data,                sizeof(car->data),              1, file);
    fread(&car->quantidadeLugares,   sizeof(car->quantidadeLugares), 1, file);
    fread(&car->codLinha,            sizeof(car->codLinha),          1, file);
    fread(&car->tamanhoModelo,       sizeof(car->tamanhoModelo),     1, file);
    fread(&car->modelo,              car->tamanhoModelo,             1, file);
    fread(&car->tamanhoCategoria,    sizeof(car->tamanhoCategoria),  1, file);
    fread(&car->categoria,           car->tamanhoCategoria,          1, file);
    return car;
}

// Reads the next car from the stantart input
Car* _readCarFromCLI(Car *car)
{
    // Initializing zeroed char arrays and then reading from stdinput
    char prefixo[5] = {0};
    char data[10] = {0};
    char quantidadeLugares[MAX_STRING_SIZE] = {0};
    char codLinha[MAX_STRING_SIZE] = {0};
    char modelo[MAX_STRING_SIZE] = {0};
    char categoria[MAX_STRING_SIZE] = {0};

    // Scanning and removing possible quotations from each field
    scan_quote_string(prefixo);
    scan_quote_string(data);
    scan_quote_string(quantidadeLugares);
    scan_quote_string(codLinha);
    scan_quote_string(modelo);
    scan_quote_string(categoria);

    strncpy(car->prefixo, prefixo, sizeof(car->prefixo));

    // DATA
    if(data[0] == 0)
        fillWithGarbage(car->data, sizeof(car->data));
    else
        strncpy(car->data, data, sizeof(car->data));     
    
    // QUANTIDADE DE LUGARES
    if(*quantidadeLugares == 0)
        car->quantidadeLugares = -1;
    else
        sscanf(quantidadeLugares, "%d", &car->quantidadeLugares);

    // COD LINHA
    if(*codLinha == 0)
        car->codLinha = -1;
    else
        sscanf(codLinha, "%d", &car->codLinha);

    // MODELO
    if(*modelo == 0)
        fillWithGarbage(car->modelo, sizeof(car->modelo));
    else
        strncpy(car->modelo, modelo, sizeof(car->modelo));    

    // CATEGORIA
    if(*categoria == 0)
        fillWithGarbage(car->categoria, sizeof(car->categoria));
    else
        strncpy(car->categoria, categoria, sizeof(car->categoria));      

    // verify if the register is removed
    if(car->prefixo[0] == '*')
    {
        car->removido = REMOVED;
        leftShift(car->prefixo, 5);
    }
    else 
        car->removido = NOT_REMOVED;

    // calculates the lenght of car->modelo and car->categoria
    int lenghtModelo = strlen(car->modelo);
    int lenghtCategoria = strlen(car->categoria);
    car->tamanhoModelo = lenghtModelo;
    car->tamanhoCategoria = lenghtCategoria;

    // calculates the size of the register
    car->tamanhoRegistro = STRUCT_BASE_CAR_SIZE + lenghtModelo + lenghtCategoria;
    
    return car;
}

// Reads a car at the current file pointer from a source file. For bin files, if
// the pointer is pointing at the header, it will read the first car in the file.
Car* readCar(Car* car, FILE* file, Source from)
{
    switch (from)
    {
        case CSV:
            return _readCarFromCSV(car, file);

        case BIN:
            return _readCarFromBIN(car, file);

        case CLI:
            return _readCarFromCLI(car);

        default:
            break;
    }
    return NULL;
}

// Prints a single field from a Car
void printField(CarHeader* header, Car* car, CarField field)
{
    char* fieldContent = getCarContent(car, field); // get the value of the field
    char* fieldName = getHeaderDescription(header, field); // get the description of the field
    printf("%s: %s\n", fieldName, fieldContent); // prints the field
    free(fieldContent);
    free(fieldName);
}

// Prints Car. Checks if Car is logically removed and also deals with nulls.
int printCar(Car* car, CarHeader* header)
{
    // verify id the car is removed
    if(car->removido == REMOVED) 
        return 0;

    // print the fields required by the specification
    printField(header, car, PREFIXO);
    printField(header, car, MODELO);
    printField(header, car, CATEGORIA);
    printField(header, car, DATA);
    printField(header, car, QTD_LUGARES);

    return 1;
}

// Free all memory associated with a Car.
void freeCar(Car* c)
{
    free(c);
}

// Free all memory associated with a CarHeader.
void freeCarHeader(CarHeader* carHeader)
{
    free(carHeader);
}

/* ## Functions related to writing Cars to different sources ## */

// Writes a Car to end of binary file.
void _writeCarToBin(Car* car, FILE* file)
{
    // gets the information from the header and update it
    CarHeader* header = newCarHeader();
    getCarHeader(header, file, BIN);
    header->status = '0';
    int byteOffset = header->byteProxReg;
    header->byteProxReg += car->tamanhoRegistro + sizeof(car->removido) + sizeof(car->tamanhoRegistro);

    // verifica se o registro é removido ou não
    if(car->removido == NOT_REMOVED)
    {
        header->nroRegistros++;
    }
    else
    {
        header->nroRegistrosRemovidos++;
    }

    // writes the header with the updated information
    writeCarHeader(header, file, BIN);
    freeCarHeader(header);
    
    // set the file pointer to the correct position
    fseek(file, byteOffset, SEEK_SET);

    // write the data in the file
    fwrite(&car->removido,           sizeof(car->removido),          1, file);
    fwrite(&car->tamanhoRegistro,    sizeof(car->tamanhoRegistro),   1, file);
    fwrite(&car->prefixo,            sizeof(car->prefixo),           1, file);
    fwrite(&car->data,               sizeof(car->data),              1, file);
    fwrite(&car->quantidadeLugares,  sizeof(car->quantidadeLugares), 1, file);
    fwrite(&car->codLinha,           sizeof(car->codLinha),          1, file);
    fwrite(&car->tamanhoModelo,      sizeof(car->tamanhoModelo),     1, file);
    fwrite(&car->modelo,             car->tamanhoModelo,             1, file);
    fwrite(&car->tamanhoCategoria,   sizeof(car->tamanhoCategoria),  1, file);
    fwrite(&car->categoria,          car->tamanhoCategoria,          1, file);
}

// Writes a Car to a specific source
// Currently only supports BIN files.
void writeCar(Car* car, FILE* file, Source from)
{
    switch (from)
    {
        case BIN:
            _writeCarToBin(car, file);
            break;

        default:
            break;
    }
}

/* ## Functions related to searching using a specific struct field ## */

// Checks what struct field to use in search, 
// Returns CarField representation of struct field found 
CarField getCarField(char* providedField)
{
    if(!strcmp(providedField, "prefixo"))
    {
        return PREFIXO;
    }
    if(!strcmp(providedField, "data"))
    {
        return DATA;
    }
    if(!strcmp(providedField, "quantidadeLugares"))
    {
        return QTD_LUGARES;
    }
    if(!strcmp(providedField, "linha"))
    {
        return COD_LINHA_CAR;
    }
    if(!strcmp(providedField, "modelo"))
    {
        return MODELO;
    }
    if(!strcmp(providedField, "categoria"))
    {
        return CATEGORIA;
    }
    return 0;
}

// Get the month member given its number
void getMonthName(char* monthName, int month)
{
    switch (month)
    {
    case 1:
        strcpy(monthName, "janeiro");
        break;

    case 2:
        strcpy(monthName, "fevereiro");
        break;

    case 3:
        strcpy(monthName, "março");
        break;

    case 4:
        strcpy(monthName, "abril");
        break;

    case 5:
        strcpy(monthName, "maio");
        break;

    case 6:
        strcpy(monthName, "junho");
        break;

    case 7:
        strcpy(monthName, "julho");
        break;

    case 8:
        strcpy(monthName, "agosto");
        break;

    case 9:
        strcpy(monthName, "setembro");
        break;

    case 10:
        strcpy(monthName, "outubro");
        break;

    case 11:
        strcpy(monthName, "novembro");
        break;

    case 12:
        strcpy(monthName, "dezembro");
        break;    
    
    default:
        break;
    }
}

// Transform a date of format "YYYY-MM-DD" to "_DAY de _MONTH_NAME_ de _YEAR_".
// Example: "2010-05-21" -> "21 de maio de 2021".
void tranformDate(char* date)
{
    // verify iF the string is null
    if(*date == 0)
    {
        strcpy(date, NULL_MESSAGE);
        return;
    }

    int day, month, year;
    char* buffer = calloc(MAX_STRING_SIZE, 1);
    char* bufferPointer = buffer;
    strcpy(buffer, date);
    
    // get the year
    char* token;
    token = strsep(&buffer, "-");
    sscanf(token, "%d", &year);

    // get the month
    token = strsep(&buffer, "-");
    sscanf(token, "%d", &month);

    // get the day
    token = strsep(&buffer, "-");
    sscanf(token, "%d", &day);

    free(bufferPointer);

    // get the month name
    char monthName[15];
    getMonthName(monthName, month);

    // create the new date format
    sprintf(date, "%02d de %s de %d", day, monthName, year);
    return;
}

// Get a new string with value of the field of a Car.
// The string must be freed by the user.
char* getCarContent(Car* car, CarField field)
{
    char* string = calloc(1, MAX_STRING_SIZE);
    switch (field)
    {
    case PREFIXO:
        strncpy(string, car->prefixo,   sizeof(car->prefixo));
        break;

    case DATA:
        strncpy(string, car->data,      sizeof(car->data));
        tranformDate(string);
        break;
    
    case QTD_LUGARES:
        snprintf(string, MAX_STRING_SIZE, "%d", car->quantidadeLugares);
        break;
   
    case COD_LINHA_CAR:
        snprintf(string, MAX_STRING_SIZE, "%d", car->codLinha);
        break;

    case MODELO:
        strncpy(string, car->modelo,    car->tamanhoModelo);
        break;

    case CATEGORIA:
        strncpy(string, car->categoria, car->tamanhoCategoria);
        break;
    
    default:
        free(string);
        return NULL;
    }
    if(string[0] == 0 || (string[0] == '-' && string[1] == '1'))
    {
        strcpy(string, NULL_MESSAGE);
    }
    return string;
}

// Get a new string with the field description of a determined field of a CarHeader.
// The string must be freed by the user.
char* getHeaderDescription(CarHeader* header, CarField field)
{
    char* string = calloc(1, MAX_STRING_SIZE);
    switch (field)
    {
    case PREFIXO:
        strncpy(string, header->descrevePrefixo,    sizeof(header->descrevePrefixo));
        break;

    case DATA:
        strncpy(string, header->descreveData,       sizeof(header->descreveData));
        break;
    
    case QTD_LUGARES:
        strncpy(string, header->descreveLugares,    sizeof(header->descreveLugares));
        break;
   
    case COD_LINHA_CAR:
        strncpy(string, header->descreveLinha,      sizeof(header->descreveLinha));
        break;

    case MODELO:
        strncpy(string, header->descreveModelo,     sizeof(header->descreveModelo));
        break;

    case CATEGORIA:
        strncpy(string, header->descreveCategoria,  sizeof(header->descreveCategoria));
        break;
    
    default:
        free(string);
        return NULL;
    }
    if(string[0] == 0 || (string[0] == '-' && string[1] == '1'))
    {
        strcpy(string, NULL_MESSAGE);
    }
    return string;
}

// Returns a CarSearchable union given a CarField.
CarSearchable CarSearchUsing(CarField field)
{
    char tmp[MAX_STRING_SIZE];
    scan_quote_string(tmp);

    CarSearchable cs;
    switch (field)
    {
    case PREFIXO:
        strncpy(cs.prefixo, tmp, sizeof(cs.prefixo));
        break;

    case DATA:
        strncpy(cs.data, tmp, sizeof(cs.data));
        break;
    
    case QTD_LUGARES:
        cs.quantidadeLugares = atoi(tmp);
        break;

    case MODELO:
        strncpy(cs.modelo, tmp, sizeof(cs.modelo));
        break;

    case CATEGORIA:
        strncpy(cs.categoria, tmp, sizeof(cs.categoria));
        break;
    
    default:
        break;
    }
    
    return cs;
}

// Check if the the car match the search.
// Returns true if it is a match, false if it isn't.
bool checkIfCarMatches(Car* car, CarField field, CarSearchable search)
{
    switch (field)
    {
    case PREFIXO:
        return !strncmp(car->prefixo, search.prefixo, sizeof(car->prefixo));

    case DATA:
        return !strncmp(car->data, search.data, sizeof(car->data));
    
    case QTD_LUGARES:
        return car->quantidadeLugares == search.quantidadeLugares;

    case MODELO:
        return !strncmp(car->modelo, search.modelo, sizeof(car->modelo));

    case CATEGORIA:
        return !strncmp(car->categoria, search.categoria, sizeof(car->categoria));
    
    default:
        break;
    }

    return UNKNOWN_ERR;
}