#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_ARGUMENTS,
    ERROR_FILE_OPEN,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_SYNTAX,
    ERROR_UNKNOWN_COMMAND,
    ERROR_DIVISION_BY_ZERO,
    ERROR_VARIABLE_NOT_INITIALIZED,
    ERROR_INVALID_VARIABLE_NAME
} StatusCode;

typedef struct {
    int variables[26];
    int initialized[26];
} InterpreterState;

StatusCode interpreter_init(InterpreterState* state);
StatusCode interpreter_execute_file(const char* input_filename, const char* log_filename);
StatusCode remove_spaces(const char* input, char* output);
int is_number(const char* str);
StatusCode get_value(const InterpreterState* state, const char* token, int* result);
StatusCode write_log(FILE* log_file, int line_num, const char* command, const InterpreterState* state, const char* operation);
StatusCode evaluate_expression(const InterpreterState* state, const char* expr, int* result);
StatusCode execute_assignment(InterpreterState* state, const char* line, FILE* log_file, int line_num);
StatusCode execute_print(const InterpreterState* state, const char* line, FILE* log_file, int line_num);
StatusCode interpret(InterpreterState* state, FILE* input, FILE* log_file);


#endif