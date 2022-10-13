#include "readline.h"
#include <stdio.h>
#include <stdlib.h>

char *readline() {
    size_t bufsize = 16;
    size_t len = 0;
    char *input = malloc(sizeof(char)*bufsize);
    if (input == NULL) {
        return NULL;
    }
    char c = getc(stdin);
    while (c != '\n') {
        input[len] = c;
        len++;
        if (len == bufsize) {
            bufsize *= 2;
            input = realloc(input, sizeof(char)*bufsize);   
        }
        c = getc(stdin);
    }
    input[len] = '\0';
    return input;
}
