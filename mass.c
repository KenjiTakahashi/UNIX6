#include "mass.h"

int mass_import(char *filename) {
    DBM *db = db_open_or_create("EIS");
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        return MASS_FILE_ERROR;
    }
    char buffer[MAX_LENGTH];
    while(fgets(buffer, MAX_LENGTH, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(buffer[0] == '\0') {
            return MASS_READ_ERROR;
        }
        char *name = strtok(buffer, ";");
        if(name == NULL) {
            return MASS_READ_ERROR;
        }
        char *rest = strtok('\0', "\n");
        if(rest == NULL) {
            return MASS_READ_ERROR;
        }
        int result = db_set(db, name, rest);
        if(result < 0) {
            return MASS_STORE_ERROR;
        }
        if(result == 1) {
            printf("warning: Record \"%s\" already exists!\n", name);
        }
    }
    fclose(fp);
    db_close(db);
    return MASS_SUCCESS;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: mass <filename>\n");
    } else {
        printf("Started...\n");
        int result = mass_import(argv[1]);
        if(result == MASS_SUCCESS) {
            printf("Finished\n");
        } else {
            printf("An error occured: %d\n", result);
        }
    }
    return 0;
}
