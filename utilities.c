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
