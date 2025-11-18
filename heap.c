#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(int *a, int *b) {
    if (!a || !b) {
        return;
    }
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sift_up(Heap *h, size_t index) {
    if (!h || !h->data || index == 0) {
        return;
    }
    
    size_t parent_index = (index - 1) / 2;
    
    if (h->data[index] < h->data[parent_index]) {
        swap(&h->data[index], &h->data[parent_index]);
        sift_up(h, parent_index);
    }
}

static void sift_down(Heap *h, size_t index) {
    if (!h || !h->data || index >= h->size) {
        return;
    }
    
    size_t left_child = 2 * index + 1;
    size_t right_child = 2 * index + 2;
    size_t smallest = index;
    
    if (left_child < h->size && h->data[left_child] < h->data[smallest]) {
        smallest = left_child;
    }
    if (right_child < h->size && h->data[right_child] < h->data[smallest]) {
        smallest = right_child;
    }
    
    if (smallest != index) {
        swap(&h->data[index], &h->data[smallest]);
        sift_down(h, smallest);
    }
}

Heap create_heap(size_t initial_capacity) {
    Heap h;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
    
    if (initial_capacity > 0) {
        h.data = (int*)malloc(initial_capacity * sizeof(int));
        if (h.data) {
            h.capacity = initial_capacity;
        }
    }
    
    return h;
}

void delete_heap(Heap *h) {
    if (!h) {
        return;
    }
    
    if (h->data) {
        free(h->data);
        h->data = NULL;
    }
    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h) {
    return !h || h->size == 0;
}

size_t size_heap(const Heap *h) {
    return h ? h->size : 0;
}

int peek_heap(const Heap *h) {
    if (!h || is_empty_heap(h)) {
        return -1;
    }
    return h->data[0];
}

void push_heap(Heap *h, int value) {
    if (!h) {
        return;
    }
    
    if (h->size >= h->capacity) {
        size_t new_capacity = h->capacity == 0 ? 1 : h->capacity * 2;
        int *new_data = (int*)realloc(h->data, new_capacity * sizeof(int));
        if (!new_data) {
            return;
        }
        h->data = new_data;
        h->capacity = new_capacity;
    }
    
    h->data[h->size] = value;
    h->size++;
    
    sift_up(h, h->size - 1);
}

int pop_heap(Heap *h) {
    if (!h || is_empty_heap(h)) {
        return -1;
    }
    
    int root_value = h->data[0];
    
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        sift_down(h, 0);
    }
    
    return root_value;
}

Heap build_heap(const int *array, size_t n) {
    Heap h = create_heap(n);
    if (!h.data || h.capacity < n) {
        return h;
    }
    
    for (size_t i = 0; i < n; i++) {
        h.data[i] = array[i];
    }
    h.size = n;
    
    for (int i = (int)n / 2 - 1; i >= 0; i--) {
        sift_down(&h, (size_t)i);
    }
    
    return h;
}

int is_equal_heap(const Heap *h1, const Heap *h2) {
    if (!h1 || !h2) {
        return 0;
    }
    if (h1->size != h2->size) {
        return 0;
    }
    
    for (size_t i = 0; i < h1->size; i++) {
        if (h1->data[i] != h2->data[i]) {
            return 0;
        }
    }
    return 1;
}