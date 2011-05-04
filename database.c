#include "database.h"

DBM *db_open_or_create(char *name) {
    return dbm_open(name, O_RDWR | O_CREAT, 0660);
}
void db_close(DBM *db) {
    dbm_close(db);
}
int db_set(DBM *db, char *key, char *value) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    datum dv;
    dv.dptr = value;
    dv.dsize = strlen(value) + 1;
    int pag = dbm_dirfno(db);
    __wlock(pag);
    int result = dbm_store(db, dk, dv, DBM_INSERT);
    __ulock(pag);
    return result;
}
char *db_get_one(DBM *db, char *key) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    int pag = dbm_dirfno(db);
    __rlock(pag);
    datum dv = dbm_fetch(db, dk);
    __ulock(pag);
    return dv.dptr;
}
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
    int pag = dbm_dirfno(db);
    __rlock(pag);
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
    __ulock(pag);
    regfree(&re);
    return i;
}
int db_remove(DBM *db, char *key) {
    datum dk;
    dk.dptr = key;
    dk.dsize = strlen(key) + 1;
    int pag = dbm_dirfno(db);
    lockf(pag, F_LOCK, 0);
    int result = dbm_delete(db, dk);
    lockf(pag, F_ULOCK, 0);
    return result;
}

struct flock *__file_lock(short type, short whence) {
    static struct flock ret;
    ret.l_type = type;
    ret.l_start = 0;
    ret.l_whence = whence;
    ret.l_len = 0;
    ret.l_pid = getpid();
    return &ret;
}
void __rlock(int fd) {
    fcntl(fd, F_SETLKW, __file_lock(F_RDLCK, SEEK_SET));
}
void __wlock(int fd) {
    fcntl(fd, F_SETLKW, __file_lock(F_WRLCK, SEEK_SET));
}
void __ulock(int fd) {
    fcntl(fd, F_SETLKW, __file_lock(F_UNLCK, SEEK_SET));
}
