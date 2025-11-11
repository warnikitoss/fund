#include <stdio.h>
#define VECTOR_TYPE int
#include "vector.h"

int copy_int(int value) {
    return value;
}

void delete_int(int value) {
    (void)value;
}

int main() {
    Vector vec = create_vector(10, copy_int, delete_int);
    
    for (int i = 0; i < 5; i++) {
        push_back_vector(&vec, i * 10);
    }
    
    for (size_t i = 0; i < vec.size; i++) {
        printf("Element at %zu: %d\n", i, get_at_vector(&vec, i));
    }
    
    Vector *vec_copy = copy_vector_new(&vec);
    
    printf("Vectors are equal: %d\n", is_equal_vector(&vec, vec_copy));
    
    delete_at_vector(&vec, 2);

    for (size_t i = 0; i < vec.size; i++) {
        printf("Element at %zu: %d\n", i, get_at_vector(&vec, i));
    }
    
    delete_vector(&vec);
    delete_vector(vec_copy);
    free(vec_copy);
    
    return 0;
}