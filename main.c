#include "main.h"

int main() {
    /*DBM *db = db_open_or_create("TEST");
    db_set(db, "kluc", "wartosc");
    char *res;
    if((res = db_get_one(db, "klucz")) != NULL) {
        printf("%s\n", res);
    }
    if((res = db_get_one(db, "NONE")) == NULL) {
        printf("NOT FOUND\n");
    }
    db_set(db, "klucz2", "2");
    db_set(db, "klucz3", "3");
    db_set(db, "klucz4", "4");
    db_set(db, "klucz5", "5");
    int f;
    char **results;
    int _res = db_get_many(db, "klucz.*", &results);
    for(f = 0; f < _res; ++f) {
        printf("%s\n", results[f]);
    }
    // free'ing
    for(f = 0; f < _res; ++f) {
        free(results[f]);
    }
    free(results);
    db_close(db);
    */
    interface_initialize();
    return 0;
}
