#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "functions.h"

void test_decimal_to_base_2r_basic() {
    char* result = NULL;
    StatusCode status;
    
    status = decimal_to_base_2r(0, 2, &result);
    assert(status == OK);
    assert(strcmp(result, "0") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1, 1, &result);
    assert(status == OK);
    assert(strcmp(result, "1") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1, 2, &result);
    assert(status == OK);
    assert(strcmp(result, "1") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1, 3, &result);
    assert(status == OK);
    assert(strcmp(result, "1") == 0);
    free(result);
    result = NULL;
}

void test_decimal_to_base_2r_common_numbers() {
    char* result = NULL;
    StatusCode status;
    
    status = decimal_to_base_2r(42, 1, &result);
    assert(status == OK);
    assert(strcmp(result, "101010") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(42, 2, &result);
    assert(status == OK);
    assert(strcmp(result, "222") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(42, 3, &result);
    assert(status == OK);
    assert(strcmp(result, "52") == 0);
    free(result);
    result = NULL;
}

void test_decimal_to_base_2r_large_numbers() {
    char* result = NULL;
    StatusCode status;

    status = decimal_to_base_2r(255, 3, &result);
    assert(status == OK);
    printf("255 in base 8: %s\n", result);
    assert(strcmp(result, "377") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1024, 2, &result);
    assert(status == OK);
    printf("1024 in base 4: %s\n", result);
    assert(strcmp(result, "100000") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1024, 4, &result);
    assert(status == OK);
    printf("1024 in base 16: %s\n", result);
    assert(strcmp(result, "400") == 0);
    free(result);
    result = NULL;
}

void test_decimal_to_base_2r_hex_letters() {
    char* result = NULL;
    StatusCode status;
    status = decimal_to_base_2r(10, 4, &result);
    assert(status == OK);
    printf("10 in base 16: %s\n", result);
    assert(strcmp(result, "A") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(15, 4, &result);
    assert(status == OK);
    printf("15 in base 16: %s\n", result);
    assert(strcmp(result, "F") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(26, 4, &result);
    assert(status == OK);
    printf("26 in base 16: %s\n", result);
    assert(strcmp(result, "1A") == 0);
    free(result);
    result = NULL;
}

void test_decimal_to_base_2r_edge_cases() {
    char* result = NULL;
    StatusCode status;
    
    status = decimal_to_base_2r(31, 5, &result);
    assert(status == OK);
    printf("31 in base 32: %s\n", result);
    assert(strcmp(result, "V") == 0);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(35, 5, &result);
    assert(status == OK);
    printf("35 in base 32: %s\n", result);
    assert(strcmp(result, "13") == 0);
    free(result);
    result = NULL;
}

void test_decimal_to_base_2r_error_cases() {
    char* result = NULL;
    StatusCode status;
    
    status = decimal_to_base_2r(42, 0, &result);
    assert(status == ERROR_INVALID_INPUT);
    
    status = decimal_to_base_2r(42, 6, &result);
    assert(status == ERROR_INVALID_INPUT);
    
    status = decimal_to_base_2r(42, -1, &result);
    assert(status == ERROR_INVALID_INPUT);
    
    status = decimal_to_base_2r(42, 2, NULL);
    assert(status == ERROR_INVALID_INPUT);
}

void test_decimal_to_base_2r_memory_allocation() {
    char* result = NULL;
    StatusCode status;
    
    status = decimal_to_base_2r(12345, 3, &result);
    assert(status == OK);
    assert(result != NULL);
    free(result);
    result = NULL;
    
    status = decimal_to_base_2r(1, 5, &result);
    assert(status == OK);
    assert(result != NULL);
    free(result);
    result = NULL;
}

void run_all_tests() {
    printf("Starting tests...\n\n");
    
    test_decimal_to_base_2r_basic();
    printf("Basic tests passed!\n");
    
    test_decimal_to_base_2r_common_numbers();
    printf("Common numbers tests passed!\n");
    
    test_decimal_to_base_2r_large_numbers();
    printf("Large numbers tests passed!\n");
    
    test_decimal_to_base_2r_hex_letters();
    printf("Hex letters tests passed!\n");
    
    test_decimal_to_base_2r_edge_cases();
    printf("Edge cases tests passed!\n");
    
    test_decimal_to_base_2r_error_cases();
    printf("Error cases tests passed!\n");
    
    test_decimal_to_base_2r_memory_allocation();
    printf("Memory allocation tests passed!\n");
    
    printf("\nAll tests passed successfully!\n");
}

int main() {
    run_all_tests();
    return 0;
}