#include "interface.h"

void interface_initialize() {
    db = db_open_or_create("EIS");
    WINDOW *main_win = initscr();
    clear();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, TRUE);
    top_left = newwin(16, 40, 0, 0);
    top_right = newwin(16, 40, 0, 41);
    bottom_left = newwin(16, 40, 16, 0);
    bottom_right = newwin(16, 40, 16, 41);
    __bottom_right_initialize();
    __bottom_left_initialize();
    __top_right_initialize();
    __top_left_initialize();
    delwin(bottom_left);
    delwin(top_right);
    delwin(top_left);
    delwin(main_win);
    endwin();
    db_close(db);
}

void __top_left_initialize() {
    keypad(top_left, TRUE);
    box(top_left, 0, 0);
    mvwprintw(top_left, 1, 17, "MAIN");
    wmove(top_left, 2, 1);
    whline(top_left, 0, 38);
    __top_left_menu(0);
    wmove(top_left, 4, 18);
    // free the forms!
    FIELD *field[6];
    FORM *form = __top_right_add(&field);
    form_driver(form, REQ_FIRST_FIELD);
    int opt;
    while((opt = wgetch(top_left)) != 27) {
        int x, y;
        getyx(top_left, y, x);
        switch(opt) {
            case 13:
                if(y == 4) {
                    __top_right_add_loop(form);
                } else if(y == 7) {
                    __top_right_search_loop();
                }
                break;
            case KEY_UP:
                if(y == 7) {
                    __top_left_menu(0);
                    wmove(top_left, 4, x);
                    form = __top_right_add(&field);
                }
                break;
            case KEY_DOWN:
                if(y == 4) {
                    __top_left_menu(1);
                    wmove(top_left, 7, x);
                    __top_right_search();
                }
                break;
            default:
                break;
        }
        wrefresh(top_left);
    } /* while() */
}

void __top_left_menu(int highlight) {
    if(highlight == 0) {
        wattron(top_left, A_REVERSE);
        mvwprintw(top_left, 4, 17, "ADD");
        wattroff(top_left, A_REVERSE);
    } else {
        mvwprintw(top_left, 4, 17, "ADD");
    }
    if(highlight == 1) {
        wattron(top_left, A_REVERSE);
        mvwprintw(top_left, 7, 16, "SEARCH");
        wattroff(top_left, A_REVERSE);
    } else {
        mvwprintw(top_left, 7, 16, "SEARCH");
    }
}

void __top_right_initialize() {
    keypad(top_right, TRUE);
    box(top_right, 0, 0);
    wmove(top_right, 2, 1);
    whline(top_right, 0, 38);
    wrefresh(top_right);
}

FORM *__top_right_add(FIELD *(*field)[6]) {
    char *labels[] = {
        "Name: ",
        "Form: ",
        "Grade: ",
        "Date: ",
        "Lecturer: "
    };
    wclear(top_right);
    __top_right_initialize();
    mvwprintw(top_right, 1, 17, "ADD");
    int i;
    for(i = 0; i < 5; ++i) {
        mvwprintw(top_right, i + 4, 8, labels[i]);
        (*field)[i] = new_field(1, 14, i, 1, 0, 0);
        set_field_back((*field)[i], A_UNDERLINE);
        field_opts_off((*field)[i], O_AUTOSKIP);
    }
    (*field)[5] = NULL;
    FORM *form = new_form((*field));
    set_form_win(form, top_right);
    set_form_sub(form, derwin(top_right, 11, 15, 4, 17));
    post_form(form);
    wrefresh(top_right);
    return form;
}

void __top_right_add_loop(FORM *form) {
    form_driver(form, REQ_FIRST_FIELD);
    int opt;
    while((opt = wgetch(top_right)) != 27) {
        switch(opt) {
            case 13:
                form_driver(form, REQ_CLR_FIELD);
                // print output on status bar
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            default:
                form_driver(form, opt);
                break;
        }
        //if(opt == 13) {
        //    // output will be printed in separate window below
        //    mvwprintw(top_right, 14, 14, "ADDING... ");
        //    if(db_set(db, arr[0], arr[1]/*this will be concat 1 - 4*/) == 0) {
        //        wprintw(top_right, "DONE");
        //    } else {
        //        wprintw(top_right, "FAILED");
        //    }
        //    __top_right_add();
        //    wmove(top_right, 4, 14);
        //}
        wrefresh(top_right);
    } /* while() */
}

void __top_right_search() {
    wclear(top_right);
    __top_right_initialize();
    mvwprintw(top_right, 1, 16, "SEARCH");
    mvwprintw(top_right, 4, 8, "Single [X]");
    mvwprintw(top_right, 5, 8, "Regexp [ ]");
    mvwprintw(top_right, 7, 8, "Query: ");
    int i;
    for(i = 0; i < 17; ++i) {
        wechochar(top_right, '_');
    }
    wrefresh(top_right);
}

void __top_right_search_loop() {
    wmove(top_right, 4, 16);
    int query_type = 0;
    char *query = malloc(sizeof(char*));
    query[0] = '\0';
    while(1) {
        int opt = wgetch(top_right);
        if(opt == 13) {
            // place debug strings below
            __bottom_left_loop(query_type, query);
        } else if(opt == KEY_UP) {
            int x, y;
            getyx(top_right, y, x);
            if(y == 7) {
                wmove(top_right, 5, 16);
            } else if(y > 4) {
                wmove(top_right, y - 1, x);
            }
        } else if(opt == KEY_DOWN) {
            int x, y;
            getyx(top_right, y, x);
            if(y == 5) {
                wmove(top_right, 7, 15);
            } else if(y == 4) {
                wmove(top_right, 5, 16);
            }
        } else if(opt == KEY_LEFT) {
        } else if(opt == KEY_RIGHT) {
        } else if(opt == 32/*space*/) {
            int x, y;
            getyx(top_right, y, x);
            if(y == 5 && query_type == 0) {
                query_type = 1;
                mvwprintw(top_right, 4, 16, " ");
                mvwprintw(top_right, 5, 16, "X");
                wmove(top_right, 5, 16);
            } else if(y == 4 && query_type == 1) {
                query_type = 0;
                mvwprintw(top_right, 5, 16, " ");
                mvwprintw(top_right, 4, 16, "X");
                wmove(top_right, 4, 16);
            } else if(y == 7) {
                wechochar(top_right, opt);
            }
        } else if(opt == KEY_BACKSPACE) {
            int x, y;
            getyx(top_right, y, x);
            if(y == 7 && x > 15) {
                mvwdelch(top_right, y, x - 1);
                mvwaddch(top_right, y, 31, '_');
                mvwaddch(top_right, y, 38, ' ');
                mvwaddch(top_right, y, 39, '│'); // think how it can be changed
                wmove(top_right, y, x - 1);
            }
        } else if(opt == 27) {
            break;
        } else {
            int x, y;
            getyx(top_right, y, x);
            if(y == 7) {
                int size = strlen(query);
                query = realloc(query, sizeof(char*) * (size + 2));
                query[size] = opt;
                query[size + 1] = '\0';
                wechochar(top_right, opt);
            }
        }
        wrefresh(top_right);
    }
}

void __bottom_left_initialize() {
    keypad(bottom_left, TRUE);
    box(bottom_left, 0, 0);
    mvwprintw(bottom_left, 1, 16, "RESULTS");
    wmove(bottom_left, 2, 1);
    whline(bottom_left, 0, 38);
    wrefresh(bottom_left);
}

void __bottom_left_loop(int query_type, char *query) {
    WINDOW *pad = newpad(200, 40);
    int position = 0;
    int r_size = 1;
    int highlight = 0;
    char **keys;
    char **results;
    if(query_type == 0) {
        char *result = db_get_one(db, query);
        mvwprintw(pad, 0, 0, "%s", result);
    } else {
        r_size = db_get_many(db, query, &keys, &results);
        __bottom_left_print(pad, keys, r_size, highlight);
        //free keys and results!
    }
    prefresh(pad, position, 0, 20, 1, 30, 38);
    while(1) {
        int opt = wgetch(bottom_left);
        if(opt == 13) {
        } else if(opt == KEY_UP) {
            int x, y;
            getyx(pad, y, x);
            if(y > 0) {
                --highlight;
                __bottom_left_print(pad, keys, r_size, highlight);
                if(y == position) {
                    --position;
                }
            }
        } else if(opt == KEY_DOWN) {
            int x, y;
            getyx(pad, y, x);
            if(y < r_size - 1) {
                ++highlight;
                __bottom_left_print(pad, keys, r_size, highlight);
            }
            if(y >= 10 && y < r_size - 1 && r_size > 11 && position < y - 9) {
                ++position;
            }
        } else if(opt == 27) {
            break;
        }
        prefresh(pad, position, 0, 20, 1, 30, 38);
    }
    delwin(pad);
}

void __bottom_left_print(WINDOW *pad, char **keys, int size, int highlight) {
    int i;
    for(i = 0; i < size; ++i) {
        if(i == highlight) {
            wattron(pad, A_REVERSE);
            mvwprintw(pad, i, 0, "%s", keys[i]);
            wattroff(pad, A_REVERSE);
        } else {
            mvwprintw(pad, i, 0, "%s", keys[i]);
        }
    }
    wmove(pad, highlight, 0);
}

void __bottom_right_initialize() {
    keypad(bottom_right, TRUE);
    box(bottom_right, 0, 0);
    mvwprintw(bottom_right, 1, 17, "EDIT");
    wmove(bottom_right, 2, 1);
    whline(bottom_right, 0, 38);
    wrefresh(bottom_right);
}
