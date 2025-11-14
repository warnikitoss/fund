#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "functions.h"

void test_remove_spaces() {
    char output[100];
    StatusCode status;
    
    status = remove_spaces("A = 5", output);
    assert(status == SUCCESS);
    assert(strcmp(output, "A=5") == 0);
    
    status = remove_spaces("", output);
    assert(status == SUCCESS);
    assert(strcmp(output, "") == 0);
    
    status = remove_spaces("   ", output);
    assert(status == SUCCESS);
    assert(strcmp(output, "") == 0);
    
    status = remove_spaces(NULL, output);
    assert(status == ERROR_INVALID_ARGUMENTS);
    
    status = remove_spaces("test", NULL);
    assert(status == ERROR_INVALID_ARGUMENTS);
}

void test_is_number() {
    assert(is_number("123") == 1);
    assert(is_number("0") == 1);
    assert(is_number("") == 0);
    assert(is_number("12a3") == 0);
    assert(is_number("12.3") == 0);
    assert(is_number("-123") == 0);
}

void test_get_value() {
    InterpreterState state;
    interpreter_init(&state);
    
    state.variables[0] = 42;
    state.initialized[0] = 1;
    state.variables[1] = 100;
    state.initialized[1] = 1;
    
    int result;
    StatusCode status;
    
    status = get_value(&state, "123", &result);
    assert(status == SUCCESS);
    assert(result == 123);
    
    status = get_value(&state, "A", &result);
    assert(status == SUCCESS);
    assert(result == 42);
    
    status = get_value(&state, "C", &result);
    assert(status == ERROR_VARIABLE_NOT_INITIALIZED);
    
    status = get_value(&state, "ABC", &result);
    assert(status == ERROR_INVALID_SYNTAX);
    
    status = get_value(NULL, "A", &result);
    assert(status == ERROR_INVALID_ARGUMENTS);
    status = get_value(&state, NULL, &result);
    assert(status == ERROR_INVALID_ARGUMENTS);
    status = get_value(&state, "A", NULL);
    assert(status == ERROR_INVALID_ARGUMENTS);
}

void test_evaluate_expression() {
    InterpreterState state;
    interpreter_init(&state);
    
    state.variables[0] = 10;
    state.initialized[0] = 1;
    state.variables[1] = 2;
    state.initialized[1] = 1;
    
    int result;
    StatusCode status;
    
    status = evaluate_expression(&state, "5+3", &result);
    assert(status == SUCCESS);
    assert(result == 8);
    
    status = evaluate_expression(&state, "A+B", &result);
    assert(status == SUCCESS);
    assert(result == 12);
    
    status = evaluate_expression(&state, "10-4", &result);
    assert(status == SUCCESS);
    assert(result == 6);
    
    status = evaluate_expression(&state, "6*7", &result);
    assert(status == SUCCESS);
    assert(result == 42);
    
    status = evaluate_expression(&state, "20/5", &result);
    assert(status == SUCCESS);
    assert(result == 4);
    
    status = evaluate_expression(&state, "10/0", &result);
    assert(status == ERROR_DIVISION_BY_ZERO);
    
    status = evaluate_expression(&state, "2^3", &result);
    assert(status == SUCCESS);
    assert(result == 8);
    
    status = evaluate_expression(&state, "A+", &result);
    assert(status == ERROR_INVALID_SYNTAX);
    
    status = evaluate_expression(NULL, "5+3", &result);
    assert(status == ERROR_INVALID_ARGUMENTS);
}

void test_execute_assignment() {
    InterpreterState state;
    interpreter_init(&state);
    FILE* log_file = fopen("test_log.txt", "w");
    
    StatusCode status;
    
    status = execute_assignment(&state, "X = 5 + 3", log_file, 1);
    assert(status == SUCCESS);
    assert(state.initialized['X' - 'A'] == 1);
    assert(state.variables['X' - 'A'] == 8);
    
    status = execute_assignment(&state, "x = 5", log_file, 2);
    assert(status == ERROR_INVALID_VARIABLE_NAME);
    
    status = execute_assignment(&state, "A = ", log_file, 3);
    assert(status == ERROR_INVALID_SYNTAX);
    
    status = execute_assignment(NULL, "A=5", log_file, 4);
    assert(status == ERROR_INVALID_ARGUMENTS);
    
    fclose(log_file);
    remove("test_log.txt");
}

void test_execute_print() {
    InterpreterState state;
    interpreter_init(&state);
    FILE* log_file = fopen("test_log.txt", "w");
    
    state.variables[0] = 42;
    state.initialized[0] = 1;
    
    StatusCode status;
    
    status = execute_print(&state, "print(A)", log_file, 1);
    assert(status == SUCCESS);
    
    status = execute_print(&state, "print(B)", log_file, 2);
    assert(status == ERROR_VARIABLE_NOT_INITIALIZED);
    
    status = execute_print(&state, "print(a)", log_file, 3);
    assert(status == ERROR_INVALID_VARIABLE_NAME);
    
    status = execute_print(&state, "print A", log_file, 4);
    assert(status == ERROR_INVALID_SYNTAX);
    
    status = execute_print(NULL, "print(A)", log_file, 5);
    assert(status == ERROR_INVALID_ARGUMENTS);
    
    fclose(log_file);
    remove("test_log.txt");
}

void test_interpreter_init() {
    InterpreterState state;
    StatusCode status;
    
    status = interpreter_init(&state);
    assert(status == SUCCESS);
    
    for (int i = 0; i < VAR_COUNT; i++) {
        assert(state.initialized[i] == 0);
    }
    
    status = interpreter_init(NULL);
    assert(status == ERROR_INVALID_ARGUMENTS);
}

void run_unit_tests() {
    test_remove_spaces();
    test_is_number();
    test_get_value();
    test_evaluate_expression();
    test_execute_assignment();
    test_execute_print();
    test_interpreter_init();
    printf("All unit tests passed!\n");
}

int main() {
    run_unit_tests();
    return 0;
}