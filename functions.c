#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define VAR_COUNT 26
#define MAX_LINE 256

StatusCode remove_spaces(const char* input, char* output) {
    if (input == NULL || output == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    const char* src = input;
    char* dst = output;
    
    while (*src) {
        if (*src != ' ') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    
    return SUCCESS;
}

int is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    for (const char* p = str; *p; p++) {
        if (!isdigit(*p)) {
            return 0;
        }
    }
    return 1;
}

StatusCode get_value(const InterpreterState* state, const char* token, int* result) {
    if (state == NULL || token == NULL || result == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    if (is_number(token)) {
        *result = atoi(token);
        return SUCCESS;
    }
    
    if (isupper(token[0]) && strlen(token) == 1) {
        int idx = token[0] - 'A';
        if (state->initialized[idx]) {
            *result = state->variables[idx];
            return SUCCESS;
        } else {
            return ERROR_VARIABLE_NOT_INITIALIZED;
        }
    }
    
    return ERROR_INVALID_SYNTAX;
}

StatusCode write_log(FILE* log_file, int line_num, const char* command, const InterpreterState* state, const char* operation) {
    if (log_file == NULL || command == NULL || state == NULL || operation == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    fprintf(log_file, "[%d] %s | ", line_num, command);
    
    int first = 1;
    for (int i = 0; i < VAR_COUNT; i++) {
        if (state->initialized[i]) {
            if (!first) fprintf(log_file, ", ");
            fprintf(log_file, "%c=%d", 'A' + i, state->variables[i]);
            first = 0;
        }
    }
    
    fprintf(log_file, " | %s\n", operation);
    return SUCCESS;
}

StatusCode evaluate_expression(const InterpreterState* state, const char* expr, int* result) {
    if (state == NULL || expr == NULL || result == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    char* tokens[VAR_COUNT];
    int token_count = 0;
    char expr_copy[MAX_LINE];
    
    strncpy(expr_copy, expr, MAX_LINE - 1);
    expr_copy[MAX_LINE - 1] = '\0';
    
    char* token = strtok(expr_copy, "+-*/^");
    while (token != NULL && token_count < VAR_COUNT) {
        tokens[token_count++] = token;
        token = strtok(NULL, "+-*/^");
    }
    
    if (token_count == 0) {
        return ERROR_INVALID_SYNTAX;
    }
    
    StatusCode status;
    int current_value;
    
    status = get_value(state, tokens[0], &current_value);
    if (status != SUCCESS) {
        return status;
    }
    
    const char* ptr = expr;
    int token_index = 1;
    
    while (*ptr && token_index < token_count) {
        if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/' || *ptr == '^') {
            char op = *ptr;
            int next_value;
            
            status = get_value(state, tokens[token_index++], &next_value);
            if (status != SUCCESS) {
                return status;
            }
            
            switch (op) {
                case '+': 
                    current_value += next_value; 
                    break;
                case '-': 
                    current_value -= next_value; 
                    break;
                case '*': 
                    current_value *= next_value; 
                    break;
                case '/': 
                    if (next_value == 0) {
                        return ERROR_DIVISION_BY_ZERO;
                    }
                    current_value /= next_value; 
                    break;
                case '^': 
                    current_value = (int)pow(current_value, next_value); 
                    break;
            }
        }
        ptr++;
    }
    
    *result = current_value;
    return SUCCESS;
}

StatusCode execute_assignment(InterpreterState* state, const char* line, FILE* log_file, int line_num) {
    if (state == NULL || line == NULL || log_file == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    char processed_line[MAX_LINE];
    StatusCode status = remove_spaces(line, processed_line);
    if (status != SUCCESS) return status;
    
    char var;
    char expr[MAX_LINE];
    
    if (sscanf(processed_line, "%c=%s", &var, expr) != 2) {
        return ERROR_INVALID_SYNTAX;
    }
    
    if (!isupper(var)) {
        return ERROR_INVALID_VARIABLE_NAME;
    }
    
    int result;
    status = evaluate_expression(state, expr, &result);
    if (status != SUCCESS) return status;
    
    int var_idx = var - 'A';
    state->variables[var_idx] = result;
    state->initialized[var_idx] = 1;
    
    return write_log(log_file, line_num, line, state, "Assignment");
}

StatusCode execute_print(const InterpreterState* state, const char* line, FILE* log_file, int line_num) {
    if (state == NULL || line == NULL || log_file == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    char processed_line[MAX_LINE];
    StatusCode status = remove_spaces(line, processed_line);
    if (status != SUCCESS) {
        return status;
    }
    
    char var;
    if (sscanf(processed_line, "print(%c)", &var) != 1) {
        return ERROR_INVALID_SYNTAX;
    }
    
    if (!isupper(var)) {
        return ERROR_INVALID_VARIABLE_NAME;
    }
    
    int var_idx = var - 'A';
    if (!state->initialized[var_idx]) {
        return ERROR_VARIABLE_NOT_INITIALIZED;
    }
    
    printf("%d\n", state->variables[var_idx]);
    return write_log(log_file, line_num, line, state, "Print");
}

StatusCode interpret(InterpreterState* state, FILE* input, FILE* log_file) {
    if (state == NULL || input == NULL || log_file == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    char line[MAX_LINE];
    int line_num = 1;
    
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            line_num++;
            continue;
        }
        
        StatusCode status;
        char processed_line[MAX_LINE];
        
        status = remove_spaces(line, processed_line);
        if (status != SUCCESS) {
            return status;
        }
        
        if (strchr(processed_line, '=') != NULL) {
            status = execute_assignment(state, line, log_file, line_num);
        } else if (strncmp(processed_line, "print(", 6) == 0) {
            status = execute_print(state, line, log_file, line_num);
        } else {
            return ERROR_UNKNOWN_COMMAND;
        }
        
        if (status != SUCCESS) return status;
        line_num++;
    }
    
    return SUCCESS;
}

StatusCode interpreter_init(InterpreterState* state) {
    if (state == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    for (int i = 0; i < VAR_COUNT; i++) {
        state->variables[i] = 0;
        state->initialized[i] = 0;
    }
    
    return SUCCESS;
}

StatusCode interpreter_execute_file(const char* input_filename, const char* log_filename) {
    if (input_filename == NULL || log_filename == NULL) {
        return ERROR_INVALID_ARGUMENTS;
    }
    
    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* log_file = fopen(log_filename, "w");
    if (!log_file) {
        fclose(input_file);
        return ERROR_FILE_OPEN;
    }
    
    InterpreterState state;
    StatusCode status = interpreter_init(&state);
    if (status != SUCCESS) {
        fclose(input_file);
        fclose(log_file);
        return status;
    }
    
    status = interpret(&state, input_file, log_file);
    
    fclose(input_file);
    fclose(log_file);
    
    return status;
}