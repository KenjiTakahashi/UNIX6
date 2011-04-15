#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <regex.h>
#ifndef __linux__
#include <ndbm.h>
#else
#define DB_DBM_HSEARCH 1
#include <db.h>
#endif

DBM *db_open_or_create(char *name);
void db_close(DBM *db);
int db_set(DBM *db, char *key, char *value);
char *db_get_one(DBM *db, char *key);
int db_get_many(DBM *db, char *pattern, char ***results); /* for regexp search. */
int db_remove(DBM *db, char *key);
