#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define VECTOR_TYPE int
#include "vector.h"

int copy_int(int value) {
    return value;
}

void delete_int(int value) {
    (void)value;
}

void test_create_vector() {
    printf("Testing create_vector... ");
    
    Vector vec = create_vector(10, copy_int, delete_int);
    assert(vec.capacity == 10);
    assert(vec.size == 0);
    assert(vec.data != NULL);
    assert(vec.CopyVoidPtr == copy_int);
    assert(vec.DeleteVoidPtr == delete_int);
    
    delete_vector(&vec);
    
    Vector vec2 = create_vector(0, copy_int, delete_int);
    assert(vec2.capacity == 0);
    assert(vec2.size == 0);
    assert(vec2.data == NULL);
    
    delete_vector(&vec2);
    
    printf("PASSED\n");
}

void test_push_back_vector() {
    printf("Testing push_back_vector... ");
    
    Vector vec = create_vector(2, copy_int, delete_int);
    
    push_back_vector(&vec, 10);
    assert(vec.size == 1);
    assert(vec.data[0] == 10);
    
    push_back_vector(&vec, 20);
    assert(vec.size == 2);
    assert(vec.data[1] == 20);
    
    push_back_vector(&vec, 30);
    assert(vec.size == 3);
    assert(vec.capacity >= 3);
    assert(vec.data[2] == 30);
    
    delete_vector(&vec);
    
    push_back_vector(NULL, 10);
    
    printf("PASSED\n");
}

void test_get_at_vector() {
    printf("Testing get_at_vector... ");
    
    Vector vec = create_vector(5, copy_int, delete_int);
    
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec, i * 10);
    }

    for (int i = 0; i < 5; i++) {
        assert(get_at_vector(&vec, i) == i * 10);
    }
    
    int value = get_at_vector(&vec, 10);
    assert(value == 0);

    value = get_at_vector(NULL, 0);
    assert(value == 0);
    
    delete_vector(&vec);
    
    printf("PASSED\n");
}

void test_delete_at_vector() {
    printf("Testing delete_at_vector... ");
    
    Vector vec = create_vector(5, copy_int, delete_int);
    
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec, i * 10);
    }
    
    delete_at_vector(&vec, 2);
    assert(vec.size == 4);
    assert(get_at_vector(&vec, 0) == 0);
    assert(get_at_vector(&vec, 1) == 10);
    assert(get_at_vector(&vec, 2) == 30);
    assert(get_at_vector(&vec, 3) == 40);
    
    delete_at_vector(&vec, 0);
    assert(vec.size == 3);
    assert(get_at_vector(&vec, 0) == 10);
    
    delete_at_vector(&vec, 2);
    assert(vec.size == 2);
    assert(get_at_vector(&vec, 0) == 10);
    assert(get_at_vector(&vec, 1) == 30);
    
    delete_at_vector(&vec, 10);
    
    delete_at_vector(NULL, 0);
    
    delete_vector(&vec);
    
    printf("PASSED\n");
}

void test_copy_vector() {
    printf("Testing copy_vector... ");
    
    Vector vec1 = create_vector(5, copy_int, delete_int);
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec1, i * 10);
    }
    
    Vector vec2 = create_vector(0, copy_int, delete_int);
    copy_vector(&vec2, &vec1);
    
    assert(vec2.size == vec1.size);
    assert(vec2.capacity == vec1.capacity);
    for (size_t i = 0; i < vec1.size; i++) {
        assert(get_at_vector(&vec2, i) == get_at_vector(&vec1, i));
    }
    
    assert(vec2.data != vec1.data);
    
    copy_vector(NULL, &vec1);
    copy_vector(&vec2, NULL);
    
    delete_vector(&vec1);
    delete_vector(&vec2);
    
    printf("PASSED\n");
}

void test_copy_vector_new() {
    printf("Testing copy_vector_new... ");
    
    Vector vec1 = create_vector(5, copy_int, delete_int);
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec1, i * 10);
    }
    
    Vector *vec_copy = copy_vector_new(&vec1);
    assert(vec_copy != NULL);
    assert(vec_copy->size == vec1.size);
    assert(vec_copy->capacity == vec1.capacity);
    
    for (size_t i = 0; i < vec1.size; i++) {
        assert(get_at_vector(vec_copy, i) == get_at_vector(&vec1, i));
    }
    
    assert(vec_copy->data != vec1.data);

    Vector *null_copy = copy_vector_new(NULL);
    assert(null_copy == NULL);
    
    delete_vector(&vec1);
    delete_vector(vec_copy);
    free(vec_copy);
    
    printf("PASSED\n");
}

void test_is_equal_vector() {
    printf("Testing is_equal_vector... ");
    
    Vector vec1 = create_vector(5, copy_int, delete_int);
    Vector vec2 = create_vector(5, copy_int, delete_int);
    
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec1, i * 10);
        push_back_vector(&vec2, i * 10);
    }
    
    assert(is_equal_vector(&vec1, &vec2) == 1);

    push_back_vector(&vec1, 50);
    assert(is_equal_vector(&vec1, &vec2) == 0);
    
    delete_at_vector(&vec1, 5);
    push_back_vector(&vec2, 60);
    assert(is_equal_vector(&vec1, &vec2) == 0);
    
    assert(is_equal_vector(NULL, &vec2) == 0);
    assert(is_equal_vector(&vec1, NULL) == 0);
    assert(is_equal_vector(NULL, NULL) == 0);
    
    delete_vector(&vec1);
    delete_vector(&vec2);
    
    printf("PASSED\n");
}

void test_erase_vector() {
    printf("Testing erase_vector... ");
    
    Vector vec = create_vector(5, copy_int, delete_int);
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec, i * 10);
    }
    
    erase_vector(&vec);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    erase_vector(&vec);
    
    erase_vector(NULL);
    
    printf("PASSED\n");
}

void test_delete_vector() {
    printf("Testing delete_vector... ");
    
    Vector vec = create_vector(5, copy_int, delete_int);
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec, i * 10);
    }
    
    delete_vector(&vec);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    
    delete_vector(&vec);
    
    delete_vector(NULL);
    
    printf("PASSED\n");
}

void run_all_tests() {
    test_create_vector();
    test_push_back_vector();
    test_get_at_vector();
    test_delete_at_vector();
    test_copy_vector();
    test_copy_vector_new();
    test_is_equal_vector();
    test_erase_vector();
    test_delete_vector();
    printf("All tests passed!\n");
}

int main() {
    run_all_tests();
    return 0;
}