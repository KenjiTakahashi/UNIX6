#include "interface.h"

void interface_initialize() {
    WINDOW *main_win = initscr();
    clear();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, TRUE);
    top_left = newwin(16, 40, 0, 0);
    top_right = newwin(16, 40, 0, 41);
    __top_right_initialize();
    __top_left_initialize();
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
        } else if(opt == 'q') {
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
    int ikses[] = {14, 14, 15, 14, 18};
    while(1) {
        int opt = wgetch(top_right);
        if(opt == 13) {
            // potwierdzenie
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
            // turn left
        } else if(opt == KEY_RIGHT) {
            // turn right
        } else if(opt == 27) {
            break;
        } else { // jakies zabezpieczenie przed zjebanymi znakami?
            // save whole string somewhere and przewijac dzemorem!
            wechochar(top_right, opt);
        }
        wrefresh(top_right);
    }
}

void __top_right_search() {
    wclear(top_right);
    __top_right_initialize();
    mvwprintw(top_right, 1, 16, "SEARCH");
    wrefresh(top_right);
}

void __top_right_search_loop() {
}

void __bottom_left_initialize() {
}

void __bottom_right_initialize() {
}
