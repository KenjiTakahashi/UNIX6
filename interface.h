#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"

WINDOW *top_left, *top_right, *bottom_left, *bottom_right;
DBM *db;

void interface_initialize();

void __top_left_initialize();
void __top_left_menu(int);

void __top_right_initialize();
void __top_right_add();
void __top_right_add_loop();
void __top_right_search();
void __top_right_search_loop();

void __bottom_left_initialize();
void __bottom_left_loop(int, char*);

void __bottom_right_initialize();
