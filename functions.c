#include <stdio.h>
#include <stdlib.h>

typedef struct {
    VECTOR_TYPE *data;
    size_t size;
    size_t capacity;
    VECTOR_TYPE (*CopyVoidPtr)(VECTOR_TYPE);
    void (*DeleteVoidPtr)(VECTOR_TYPE);
} Vector;

Vector create_vector(size_t initial_capacity, VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), void (*DeleteFunc)(VECTOR_TYPE)) {
    Vector v;
    v.capacity = initial_capacity;
    v.size = 0;

    if (CopyFunc) {
        v.CopyVoidPtr = CopyFunc;
    } else {
        v.CopyVoidPtr = default_copy;
    }
    
    if (DeleteFunc) {
        v.DeleteVoidPtr = DeleteFunc;
    } else {
        v.DeleteVoidPtr = default_delete;
    }

    if (initial_capacity > 0) {
        v.data = (VECTOR_TYPE*)malloc(initial_capacity * sizeof(VECTOR_TYPE));
        if (!v.data) {
            v.capacity = 0;
        }
    } else {
        v.data = NULL;
    }

    return v;
}

void erase_vector(Vector *v) {
    if (v && v->data) {
        for (size_t i = 0; i < v->size; i++) {
            v->DeleteVoidPtr(v->data[i]);
        }
        free(v->data);
        v->data = NULL;
        v->size = 0;
        v->capacity = 0;
    }
}

int is_equal_vector(const Vector *v1, const Vector *v2) {
    if (!v1 || !v2) {
        return 0;
    }
    if (v1->size != v2->size) {
        return 0;
    }
    for (size_t i = 0; i < v1->size; i++) {
        if (memcmp(&v1->data[i], &v2->data[i], sizeof(VECTOR_TYPE)) != 0) {
            return 0;
        }
    }
    return 1;
}

void copy_vector(Vector *dest, const Vector *src) {
    if (!dest || !src) {
        return;
    }
    
    erase_vector(dest);
    
    dest->size = src->size;
    dest->capacity = src->capacity;
    dest->CopyVoidPtr = src->CopyVoidPtr;
    dest->DeleteVoidPtr = src->DeleteVoidPtr;
    
    if (src->capacity > 0) {
        dest->data = (VECTOR_TYPE*)malloc(src->capacity * sizeof(VECTOR_TYPE));
        if (!dest->data) {
            dest->capacity = 0;
            dest->size = 0;
            return;
        }
        
        for (size_t i = 0; i < src->size; i++) {
            dest->data[i] = dest->CopyVoidPtr(src->data[i]);
        }
    } else {
        dest->data = NULL;
    }
}