#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <regex.h>
#if HAVE_NDBM_H
#include <ndbm.h>
#else
#define DB_DBM_HSEARCH 1
#include <db.h>
#endif

struct flock *__file_lock(short, short);
void __rlock(int);
void __wlock(int);
void __ulock(int);

DBM *db_open_or_create(char*);
void db_close(DBM*);
int db_set(DBM*, char*, char*);
char *db_get_one(DBM*, char*);
int db_get_many(DBM*, char*, char***, char***); /* for regexp search. */
int db_remove(DBM*, char*);
