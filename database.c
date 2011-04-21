#include "database.h"

DBM *db_open_or_create(char *name) {
    return dbm_open(name, O_RDWR | O_CREAT, 0660);
};
void db_close(DBM *db) {
    dbm_close(db);
};
int db_set(DBM *db, char *key, char *value) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    datum dv;
    dv.dptr = value;
    dv.dsize = strlen(value) + 1;
    return dbm_store(db, dk, dv, DBM_INSERT);
};
char *db_get_one(DBM *db, char *key) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    datum dv = dbm_fetch(db, dk);
    return dv.dptr;
};
int db_get_many(DBM *db, char *pattern, char ***keys, char ***results) {
    regex_t re;
    if(regcomp(&re, pattern, REG_EXTENDED) != 0) {
        return 0;
    }
    regmatch_t res;
    datum dk;
    int i = 0;
    *keys = malloc(1);
    *results = malloc(1);
    for(dk = dbm_firstkey(db); dk.dptr != NULL; dk = dbm_nextkey(db)) {
        datum dv = dbm_fetch(db, dk);
        if(regexec(&re, dk.dptr, (size_t)1, &res, 0) == 0) {
            *keys = realloc(*keys, sizeof(char*) * (i + 1));
            (*keys)[i] = malloc(sizeof(char*) * dk.dsize);
            strncpy((*keys)[i], dk.dptr, dk.dsize);
            *results = realloc(*results, sizeof(char*) * (i + 1));
            (*results)[i] = malloc(sizeof(char*) * dv.dsize);
            strncpy((*results)[i], dv.dptr, dv.dsize);
            i++;
        }
    }
    regfree(&re);
    return i;
};
int db_remove(DBM *db, char *key) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    return dbm_delete(db, dk);
};
