#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    v.CopyVoidPtr = CopyFunc;
    
    v.DeleteVoidPtr = DeleteFunc;

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

Vector *copy_vector_new(const Vector *src) {
    if (!src) {
        return NULL;
    }
    
    Vector *new_vec = (Vector*)malloc(sizeof(Vector));
    if (!new_vec) {
        return NULL;
    }
    
    new_vec->size = src->size;
    new_vec->capacity = src->capacity;
    new_vec->CopyVoidPtr = src->CopyVoidPtr;
    new_vec->DeleteVoidPtr = src->DeleteVoidPtr;
    
    if (src->capacity > 0) {
        new_vec->data = (VECTOR_TYPE*)malloc(src->capacity * sizeof(VECTOR_TYPE));
        if (!new_vec->data) {
            free(new_vec);
            return NULL;
        }
        
        for (size_t i = 0; i < src->size; i++) {
            new_vec->data[i] = new_vec->CopyVoidPtr(src->data[i]);
        }
    } else {
        new_vec->data = NULL;
    }
    
    return new_vec;
}

void push_back_vector(Vector *v, VECTOR_TYPE value) {
    if (!v) {
        return;
    }
    
    if (v->size >= v->capacity) {
        size_t new_capacity;
        if (v->capacity == 0) {
            new_capacity = 1;
        } else {
            new_capacity = v->capacity * 2;
        }
        VECTOR_TYPE *new_data = (VECTOR_TYPE*)realloc(v->data, new_capacity * sizeof(VECTOR_TYPE));
        if (!new_data) {
            return;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    }
    
    v->data[v->size] = v->CopyVoidPtr(value);
    v->size++;
}

void delete_at_vector(Vector *v, size_t index) {
    if (!v || index >= v->size) {
        return;
    }
    v->DeleteVoidPtr(v->data[index]);

    for (size_t i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
}

VECTOR_TYPE get_at_vector(const Vector *v, size_t index) {
    if (!v || index >= v->size) {
        VECTOR_TYPE empty;
        memset(&empty, 0, sizeof(VECTOR_TYPE));
        return empty;
    }
    return v->data[index];
}

void delete_vector(Vector *v) {
    if (v) {
        erase_vector(v);
    }
}