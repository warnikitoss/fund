#include "functions.h"
#include <stdio.h>

StatusCode decimal_to_base_2r(const unsigned int number, const int r, char **result) {
    if (r < 1 || r > 5 || result == NULL) {
        return ERROR_INVALID_INPUT;
    }
    const unsigned int base = 1 << r;
    const unsigned int mask = base - 1;
    
    if (number == 0) {
        *result = (char*)malloc(2);
        if (*result == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return OK;
    }
    
    int length = 0;
    unsigned int temp = number;
    while (temp != 0) {
        length++;
        temp >>= r;
    }
    
    *result = (char*)malloc(length + 1);
    if (*result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    temp = number;
    int pos = length;
    (*result)[pos] = '\0';
    pos--;
    
    while (temp != 0) {
        unsigned int digit = temp & mask;
        
        if (digit < 10) {
            (*result)[pos] = '0' + digit;
        } else {
            (*result)[pos] = 'A' + (digit - 10);
        }
        
        temp >>= r;
        pos--;
    }
    
    return OK;
}