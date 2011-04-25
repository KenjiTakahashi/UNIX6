#include <curses.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"
#include "utilities.h"

WINDOW *top_left, *top_right, *bottom_left, *bottom_right, *status;
DBM *db;

void interface_initialize();

void __top_left_initialize();
void __top_left_menu(int);

void __top_right_initialize();
FORM *__top_right_add(FIELD*(*)[6]); /* we need FIELDs to be alive! */
void __top_right_add_loop(FORM*, FIELD*[6]);
FORM *__top_right_search(FIELD*(*)[6]);
void __top_right_search_loop(FORM*, FIELD*[6]);

void __bottom_left_initialize();
void __bottom_left_loop(int, char*);
void __bottom_left_print(WINDOW*, char**, int, int);

void __bottom_right_initialize();
void __bottom_right_loop(FORM*, FIELD*[6]);
FORM *__bottom_right_print(FIELD*(*)[6]);
void __bottom_right_set_values(char*, char**, FIELD*[6]);

void __status_initialize();

void __free_form(FORM*, FIELD*[6], int);
