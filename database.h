#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <regex.h>
#ifndef __linux__
#include <ndbm.h>
#else
#define DB_DBM_HSEARCH 1
#include <db.h>
#endif

DBM *db_open_or_create(char*);
void db_close(DBM*);
int db_set(DBM*, char*, char*);
char *db_get_one(DBM*, char*);
int db_get_many(DBM*, char*, char***, char***); /* for regexp search. */
int db_remove(DBM*, char*);
