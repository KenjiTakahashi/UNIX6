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
    char *result = malloc(sizeof(char) * real_length);
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
            (n_size + f_size + g_size + l_size + 3));
    strncpy(result, name, n_size);
    strncat(result, ";", 1);
    strncat(result, form, f_size);
    strncat(result, ";", 1);
    strncat(result, grade, g_size);
    strncat(result, ";", 1);
    strncat(result, lecturer, l_size);
    return result;
}

char **__util_explode(char* string) {
    char **result = malloc(sizeof(char*) * 4);
    char *partial = strtok(string, ";");
    int size = strlen(partial);
    result[0] = malloc(sizeof(char) * size);
    result[0][size] = '\0';
    strncpy(result[0], partial, size);
    int i;
    for(i = 1; i < 4; ++i) {
        partial = strtok('\0', ";");
        size = strlen(partial);
        result[i] = malloc(sizeof(char) * size);
        result[i][size] = '\0';
        strncpy(result[i], partial, size);
    }
    return result;
}
