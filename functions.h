#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>

typedef enum {
    OK = 0,
    ERROR_INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION
} StatusCode;

StatusCode decimal_to_base_2r(const unsigned int number, const int r, char **result);

#endif