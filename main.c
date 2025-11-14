#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_log_file>\n", argv[0]);
        return ERROR_INVALID_ARGUMENTS;
    }
    
    StatusCode status = interpreter_execute_file(argv[1], argv[2]);
    
    switch (status) {
        case SUCCESS:
            printf("Interpretation completed successfully. Log saved to %s\n", argv[2]);
            break;
        case ERROR_INVALID_ARGUMENTS:
            printf("Error: Invalid arguments\n");
            break;
        case ERROR_FILE_OPEN:
            printf("Error: Cannot open file\n");
            break;
        case ERROR_INVALID_SYNTAX:
            printf("Error: Invalid syntax in input file\n");
            break;
        case ERROR_UNKNOWN_COMMAND:
            printf("Error: Unknown command in input file\n");
            break;
        case ERROR_DIVISION_BY_ZERO:
            printf("Error: Division by zero\n");
            break;
        case ERROR_VARIABLE_NOT_INITIALIZED:
            printf("Error: Variable not initialized\n");
            break;
        case ERROR_INVALID_VARIABLE_NAME:
            printf("Error: Invalid variable name\n");
            break;
        default:
            printf("Error: Unknown error occurred\n");
            break;
    }
    
    return status;
}