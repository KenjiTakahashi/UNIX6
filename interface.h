#include <curses.h>

WINDOW *top_left, *top_right, *bottom_left, *bottom_right;

void interface_initialize();

void __top_left_initialize();
void __top_left_menu(int);

void __top_right_initialize();
void __top_right_add();
void __top_right_add_loop();
void __top_right_search();
void __top_right_search_loop();

void __bottom_left_initialize();

void __bottom_right_initialize();
