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
    status = newwin(3, 81, 32, 0);
    __status_initialize();
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
                    __top_right_add_loop(form, field);
                } else if(y == 7) {
                    __top_right_search_loop(form, field);
                }
                break;
            case KEY_UP:
                if(y == 7) {
                    __top_left_menu(0);
                    wmove(top_left, 4, x);
                    __free_form(form, field, 1);
                    form = __top_right_add(&field);
                }
                break;
            case KEY_DOWN:
                if(y == 4) {
                    __top_left_menu(1);
                    wmove(top_left, 7, x);
                    __free_form(form, field, 5);
                    form = __top_right_search(&field);
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
    FORM *form = new_form(*field);
    set_form_win(form, top_right);
    set_form_sub(form, derwin(top_right, 11, 15, 4, 17));
    post_form(form);
    wrefresh(top_right);
    return form;
}

void __top_right_add_loop(FORM *form, FIELD *field[6]) {
    form_driver(form, REQ_FIRST_FIELD);
    int opt;
    while((opt = wgetch(top_right)) != 27) {
        int i;
        char *res, *res0, *res1, *res2, *res3, *res4;
        switch(opt) {
            case 13:
                __status_initialize();
                form_driver(form, REQ_NEXT_FIELD);
                mvwprintw(status, 1, 1, "ADDING... ");
                res0 = __util_remove_trailing_spaces(field_buffer(field[0], 0));
                res1 = __util_remove_trailing_spaces(field_buffer(field[1], 0));
                res2 = __util_remove_trailing_spaces(field_buffer(field[2], 0));
                res3 = __util_remove_trailing_spaces(field_buffer(field[3], 0));
                res4 = __util_remove_trailing_spaces(field_buffer(field[4], 0));
                res = __util_concat(res1, res2, res3, res4);
                if(db_set(db, res0, res) == 0) {
                    wprintw(status, "DONE");
                    for(i = 0; i < 5; ++i) {
                        form_driver(form, REQ_NEXT_FIELD);
                        form_driver(form, REQ_CLR_FIELD);
                    }
                } else {
                    wprintw(status, "FAILED");
                }
                wrefresh(status);
                form_driver(form, REQ_FIRST_FIELD);
                free(res0);
                free(res1);
                free(res2);
                free(res3);
                free(res4);
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            case KEY_LEFT:
                form_driver(form, REQ_LEFT_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_RIGHT_CHAR);
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_DEL_PREV);
                break;
            default:
                form_driver(form, opt);
                break;
        }
        wrefresh(top_right);
    } /* while() */
}

FORM *__top_right_search(FIELD *(*field)[6]) {
    wclear(top_right);
    __top_right_initialize();
    mvwprintw(top_right, 1, 16, "SEARCH");
    mvwprintw(top_right, 4, 8, "Single [X]");
    mvwprintw(top_right, 5, 8, "Regexp [ ]");
    mvwprintw(top_right, 7, 8, "Query: ");
    (*field)[0] = new_field(1, 18, 0, 0, 0, 0);
    set_field_back((*field)[0], A_UNDERLINE);
    field_opts_off((*field)[0], O_AUTOSKIP);
    (*field)[1] = NULL;
    FORM *form = new_form(*field);
    set_form_win(form, top_right);
    set_form_sub(form, derwin(top_right, 1, 18, 7, 15));
    post_form(form);
    wrefresh(top_right);
    return form;
}

void __top_right_search_loop(FORM *form, FIELD *field[6]) {
    wmove(top_right, 4, 16);
    int query_type = 0;
    int opt;
    while((opt = wgetch(top_right)) != 27) {
        int x, y;
        getyx(top_right, y, x);
        char *quer;
        switch(opt) {
            case 13:
                __status_initialize();
                form_driver(form, REQ_NEXT_FIELD);
                mvwprintw(status, 1, 1, "SEARCHING... ");
                wrefresh(status);
                quer = __util_remove_trailing_spaces(field_buffer(field[0], 0));
                __bottom_left_loop(query_type, quer);
                form_driver(form, REQ_END_LINE);
                free(quer);
                break;
            case KEY_UP:
                if(y == 7) {
                    wmove(top_right, 5, 16);
                } else if(y > 4) {
                    wmove(top_right, y - 1, x);
                }
                break;
            case KEY_DOWN:
                if(y == 5) {
                    form_driver(form, REQ_NEXT_FIELD);
                    form_driver(form, REQ_END_LINE);
                } else if(y == 4) {
                    wmove(top_right, 5, 16);
                }
                break;
            case KEY_LEFT:
                if(y == 7) {
                    form_driver(form, REQ_LEFT_CHAR);
                }
                break;
            case KEY_RIGHT:
                if(y == 7) {
                    form_driver(form, REQ_RIGHT_CHAR);
                }
                break;
            case KEY_BACKSPACE:
                    form_driver(form, REQ_DEL_PREV);
                break;
            case 32/*space*/:
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
                    form_driver(form, opt);
                }
                break;
            default:
                form_driver(form, opt);
                break;
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
    int pos = 0;
    int r_size = 0;
    int highlight = 0;
    char **keys;
    char **results;
    if(query_type == 0) {
        results = malloc(sizeof(char*));
        results[0] = db_get_one(db, query);
        if(results[0] != NULL) {
            r_size = 1;
            keys = malloc(sizeof(char*));
            keys[0] = malloc(sizeof(char) * (strlen(query) + 2));
            strncpy(keys[0], query, strlen(query) + 1);
            wattron(pad, A_REVERSE);
            mvwprintw(pad, 0, 0, keys[0]);
            wattroff(pad, A_REVERSE);
        }
    } else {
        r_size = db_get_many(db, query, &keys, &results);
        __bottom_left_print(pad, keys, r_size, highlight);
    }
    if(r_size != 0) {
        wprintw(status, "FOUND %d RESULT(S)", r_size);
        wrefresh(status);
        FIELD *field[6];
        FORM *form = __bottom_right_print(&field);
        char **data = __util_explode(results[0]);
        __bottom_right_set_values(keys[0], data, field);
        wmove(pad, 0, 0);
        prefresh(pad, pos, 0, 20, 1, 30, 38);
        int opt;
        while((opt = wgetch(bottom_left)) != 27) {
            int x, y;
            getyx(pad, y, x);
            switch(opt) {
                case 13:
                    __bottom_right_loop(form, field, keys[0]);
                    break;
                case KEY_UP:
                    if(y > 0) {
                        --highlight;
                        __bottom_left_print(pad, keys, r_size, highlight);
                        __util_free_exploded(data);
                        data = __util_explode(results[highlight]);
                        __bottom_right_set_values(keys[highlight], data, field);
                        if(y == pos) {
                            --pos;
                        }
                    }
                    break;
                case KEY_DOWN:
                    if(y < r_size - 1) {
                        ++highlight;
                        __bottom_left_print(pad, keys, r_size, highlight);
                        __util_free_exploded(data);
                        data = __util_explode(results[highlight]);
                        __bottom_right_set_values(keys[highlight], data, field);
                        if(y >= 10 && r_size > 11 && pos < y - 9) {
                            ++pos;
                        }
                    }
                    break;
                default:
                    break;
            }
            prefresh(pad, pos, 0, 20, 1, 30, 38);
        }
        int i;
        for(i = 0; i < r_size; ++i) {
            free(keys[i]);
            if(query_type == 1) {
                free(results[i]);
            }
        }
        free(keys);
        free(results);
        __free_form(form, field, 5);
        __util_free_exploded(data);
    } else {
        wprintw(status, "NO RESULTS FOUND");
    }
    wrefresh(status);
    delwin(pad);
}

void __bottom_left_print(WINDOW *pad, char **keys, int size, int highlight) {
    int i;
    for(i = 0; i < size; ++i) {
        if(i == highlight) {
            wattron(pad, A_REVERSE);
            mvwprintw(pad, i, 0, keys[i]);
            wattroff(pad, A_REVERSE);
        } else {
            mvwprintw(pad, i, 0, keys[i]);
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

void __bottom_right_loop(FORM *form, FIELD *field[6], char *key) {
    form_driver(form, REQ_FIRST_FIELD);
    form_driver(form, REQ_END_LINE);
    int opt;
    while((opt = wgetch(bottom_right)) != 27) {
        char *res, *res0, *res1, *res2, *res3, *res4;
        switch(opt) {
            case 13:
                __status_initialize();
                form_driver(form, REQ_NEXT_FIELD);
                mvwprintw(status, 1, 1, "EDITING... ");
                if(db_remove(db, key) == 0) {
                    res0 = __util_remove_trailing_spaces(
                            field_buffer(field[0], 0));
                    res1 = __util_remove_trailing_spaces(
                            field_buffer(field[1], 0));
                    res2 = __util_remove_trailing_spaces(
                            field_buffer(field[2], 0));
                    res3 = __util_remove_trailing_spaces(
                            field_buffer(field[3], 0));
                    res4 = __util_remove_trailing_spaces(
                            field_buffer(field[4], 0));
                    res = __util_concat(res1, res2, res3, res4);
                    if(db_set(db, res0, res) == 0) {
                        wprintw(status, "DONE");
                    } else {
                        wprintw(status, "FAILED");
                    }
                } else {
                    wprintw(status, "FAILED");
                }
                wrefresh(status);
                break;
            case KEY_UP:
                form_driver(form, REQ_PREV_FIELD);
                break;
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                break;
            case KEY_LEFT:
                form_driver(form, REQ_LEFT_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_RIGHT_CHAR);
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_DEL_PREV);
                break;
            default:
                form_driver(form, opt);
                break;
        }
    }
    wrefresh(bottom_right);
}

FORM *__bottom_right_print(FIELD *(*field)[6]) {
    char *labels[] = {
        "Name: ",
        "Form: ",
        "Grade: ",
        "Date: ",
        "Lecturer: "
    };
    int i;
    for(i = 0; i < 5; ++i) {
        mvwprintw(bottom_right, i + 4, 8, labels[i]);
        (*field)[i] = new_field(1, 14, i, 1, 0, 0);
        set_field_back((*field)[i], A_UNDERLINE);
        field_opts_off((*field)[i], O_AUTOSKIP);
    }
    (*field)[5] = NULL;
    FORM *form = new_form(*field);
    set_form_win(form, bottom_right);
    set_form_sub(form, derwin(bottom_right, 11, 15, 4, 17));
    post_form(form);
    wrefresh(bottom_right);
    return form;
}

void __bottom_right_set_values(char *key, char **data, FIELD *field[6]) {
    set_field_buffer(field[0], 0, key);
    int i;
    for(i = 0; i < 4; ++i) {
        set_field_buffer(field[i + 1], 0, data[i]);
    }
    wrefresh(bottom_right);
}

void __status_initialize() {
    wclear(status);
    box(status, 0, 0);
    wrefresh(status);
}

void __free_form(FORM *form, FIELD *field[6], int size) {
    unpost_form(form);
    free_form(form);
    int i;
    for(i = 0; i < size; ++i) {
        free_field(field[i]);
    }
}
