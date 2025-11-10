#include <string.h>
#include "functions.h"

int check_brackets(const char *str) {
    char stack[strlen(str)];
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '(' || str[i] == '<' || str[i] == '{' || str[i] == '[') {
            stack[j] = str[i];
            j++;
        } else if(str[i] == ')' || str[i] == '>' || str[i] == '}' || str[i] == ']') {
            if (j == 0) {
                return 0;
            }
            if (stack[j - 1] == '(' && str[i] == ')') {
                j--;
            } else if (stack[j - 1] == '<' && str[i] == '>') {
                j--;
            } else if (stack[j - 1] == '{' && str[i] == '}') {
                j--;
            } else if (stack[j - 1] == '[' && str[i] == ']') {
                j--;
            } else {
                return 0;
            }
        }
    }
    if (j == 0) {
        return 1;
    }
    return 0;
}