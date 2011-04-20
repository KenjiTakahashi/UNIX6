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
}

void __top_left_initialize() {
    keypad(top_left, TRUE);
    box(top_left, 0, 0);
    mvwprintw(top_left, 1, 17, "MAIN");
    wmove(top_left, 2, 1);
    whline(top_left, 0, 38);
    __top_left_menu(0);
    wmove(top_left, 4, 18);
    __top_right_add();
    while(1) {
        int opt = wgetch(top_left);
        if(opt == 13) {
            int x, y;
            getyx(top_left, y, x);
            if(y == 4) {
                __top_right_add_loop();
            } else if(y == 7) {
                __top_right_search_loop();
            }
        } else if(opt == KEY_UP) {
            int x, y;
            getyx(top_left, y, x);
            if(y == 7) {
                __top_left_menu(0);
                wmove(top_left, 4, x);
                __top_right_add();
            }
        } else if(opt == KEY_DOWN) {
            int x, y;
            getyx(top_left, y, x);
            if(y == 4) {
                __top_left_menu(1);
                wmove(top_left, 7, x);
                __top_right_search();
            }
        } else if(opt == 27) {
            break;
        }
        wrefresh(top_left);
    } /* while(1) */
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

void __top_right_add() {
    char *labels[] = {
        "Name: ",
        "Form: ",
        "Grade: ",
        "Date: ",
        "Lecturer: "
    };
    int sizes[] = {18, 18, 17, 18, 14};
    wclear(top_right);
    __top_right_initialize();
    mvwprintw(top_right, 1, 17, "ADD");
    int i;
    for(i = 0; i < 5; ++i) {
        mvwprintw(top_right, i + 4, 8, labels[i]);
        int j;
        for(j = 0; j < sizes[i]; ++j) {
            wechochar(top_right, '_');
        }
    }
    wrefresh(top_right);
}

void __top_right_add_loop() {
    wmove(top_right, 4, 14);
    char *arr[5];
    int i;
    for(i = 0; i < 5; ++i) {
        arr[i] = malloc(sizeof(char*));
        arr[i][0] = '\0';
    }
    int ikses[] = {14, 14, 15, 14, 18};
    while(1) {
        int opt = wgetch(top_right);
        if(opt == 13) {
            // output will be printed in separate window below
            mvwprintw(top_right, 14, 14, "ADDING... ");
            if(db_set(db, arr[0], arr[1]/*this will be concat 1 - 4*/) == 0) {
                wprintw(top_right, "DONE");
            } else {
                wprintw(top_right, "FAILED");
            }
            __top_right_add();
            wmove(top_right, 4, 14);
        } else if(opt == KEY_UP) {
            int x, y;
            getyx(top_right, y, x);
            if(y > 4) {
                wmove(top_right, y - 1, ikses[y - 5]);
            }
        } else if(opt == KEY_DOWN) {
            int x, y;
            getyx(top_right, y, x);
            if(y < 8) {
                wmove(top_right, y + 1, ikses[y - 3]);
            }
        } else if(opt == KEY_LEFT) {
            int x, y;
            getyx(top_right, y, x);
            if(x > ikses[y - 4]) {
                wmove(top_right, y, x - 1);
            }
        } else if(opt == KEY_RIGHT) {
            int x, y;
            getyx(top_right, y, x);
            if(x < ikses[y - 4] + strlen(arr[y - 4])) {
                wmove(top_right, y, x + 1);
            }
        } else if(opt == 27) {
            break;
        } else { // jakies zabezpieczenie przed zjebanymi znakami?
            // save whole string somewhere and przewijac dzemorem!
            int x, y;
            getyx(top_right, y, x);
            int size = strlen(arr[y - 4]);
            arr[y - 4] = realloc(arr[y - 4], sizeof(char*) * (size + 2));
            arr[y - 4][size] = opt;
            arr[y - 4][size + 1] = '\0';
            wechochar(top_right, opt);
        }
        wrefresh(top_right);
    } /* while(1) */
    for(i = 0; i < 5; ++i) {
        free(arr[i]);
    }
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

void __bottom_right_initialize() {
    keypad(bottom_right, TRUE);
    box(bottom_right, 0, 0);
    mvwprintw(bottom_right, 1, 17, "EDIT");
    wmove(bottom_right, 2, 1);
    whline(bottom_right, 0, 38);
    wrefresh(bottom_right);
}
