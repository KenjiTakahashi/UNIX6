#include <stdio.h>
#include <string.h>
#include "database.h"

#define MAX_LENGTH 1000000

#define MASS_SUCCESS 0
#define MASS_FAILURE -1
#define MASS_FILE_ERROR -2
#define MASS_READ_ERROR -3
#define MASS_STORE_ERROR -4

int mass_import(char*);
