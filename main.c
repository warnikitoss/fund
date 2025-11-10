#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void error(StatusCode a) {
    switch (a) {
        case ERROR_INVALID_INPUT:
            printf("wrong number");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("error memory allocation");
            break;
        case OK:
            break;
    }
}

int main() {
    char* result = NULL;
    StatusCode status;
    
    unsigned int numbers[] = {0, 1, 42, 255, 1024};
    int r_values[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            status = decimal_to_base_2r(numbers[i], r_values[j], &result);
            
            if (status == OK) {
                printf("%u -> 2^%d: %s\n", numbers[i], r_values[j], result);
                free(result);
                result = NULL;
            } else {
                printf("%u -> 2^%d: ", numbers[i], r_values[j]);
                error(status);
                printf("\n");
            }
        }
        printf("\n");
    }
    
    printf("Error tests:\n");
    
    status = decimal_to_base_2r(42, 0, &result);
    if (status != OK) {
        error(status);
        printf("\n");
    }
    
    status = decimal_to_base_2r(42, 6, &result);
    if (status != OK) {
        error(status);
        printf("\n");
    }
    
    status = decimal_to_base_2r(42, 2, NULL);
    if (status != OK) {
        error(status);
        printf("\n");
    }
    
    return 0;
}