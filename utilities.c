#include "utilities.h"

char *__util_remove_trailing_spaces(char *string) {
    int length = strlen(string);
    int real_length = length;
    int i;
    for(i = length - 1; i > 0; --i) {
        if(string[i] != ' ') {
            break;
        } else {
            --real_length;
        }
    }
    char *result = malloc(sizeof(char) * (real_length + 1));
    strncpy(result, string, real_length);
    result[real_length] = '\0';
    return result;
}

char *__util_concat(char *name, char *form, char *grade, char *lecturer) {
    int n_size = strlen(name);
    int f_size = strlen(form);
    int g_size = strlen(grade);
    int l_size = strlen(lecturer);
    char *result = malloc(sizeof(char) *
            (n_size + f_size + g_size + l_size + 4));
    result[0] = '\0';
    strncat(result, name, n_size);
    strncat(result, ";", 1);
    strncat(result, form, f_size);
    strncat(result, ";", 1);
    strncat(result, grade, g_size);
    strncat(result, ";", 1);
    strncat(result, lecturer, l_size);
    return result;
}

char **__util_explode(char* orig) {
    int size = strlen(orig);
    char *string = malloc(sizeof(char) * (size + 1));
    strncpy(string, orig, size);
    string[size] = '\0';
    char **result = malloc(sizeof(char*) * 5);
    char *partial = strtok(string, ";");
    int i;
    if(partial != NULL) {
        size = strlen(partial);
        result[0] = malloc(sizeof(char) * (size + 1));
        strncpy(result[0], partial, size);
        result[0][size] = '\0';
        for(i = 1; i < 4; ++i) {
            partial = strtok('\0', ";");
            if(partial != NULL) {
                size = strlen(partial);
            } else {
                size = 0;
            }
            result[i] = malloc(sizeof(char) * (size + 1));
            if(partial != NULL) {
                strncpy(result[i], partial, size);
            }
            result[i][size] = '\0';
        }
    } else {
        for(i = 0; i < 4; ++i) {
            result[i] = malloc(sizeof(char));
            result[i][0] = '\0';
        }
    }
    free(string);
    return result;
}

void __util_free_exploded(char **data) {
    int i;
    for(i = 0; i < 4; ++i) {
        free(data[i]);
    }
    free(data);
}
