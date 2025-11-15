#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <time.h>

typedef struct {
    int id;
    char surname[256];
    char name[256];
    char patronymic[256];
    struct {
        int day;
        int month;
        int year;
    } birth_date;
    char gender;
    double income;
} Liver;

typedef struct Node {
    Liver data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t size;
} LinkedList;

typedef struct {
    int type; // 0-add, 1-delete, 2-modify
    Liver liver;
    int index;
    Liver old_data; // для модификации
} Action;

LinkedList create_list(void);
void erase_list(LinkedList* list);
void delete_list(LinkedList* list);
void push_back_list(LinkedList* list, Liver value);
void push_front_list(LinkedList* list, Liver value);
Liver pop_back_list(LinkedList* list);
Liver pop_front_list(LinkedList* list);
void insert_at_list(LinkedList* list, size_t index, Liver value);
void delete_at_list(LinkedList* list, size_t index);
Liver get_at_list(const LinkedList* list, size_t index);
int is_equal_list(const LinkedList* l1, const LinkedList* l2);

void push_stack(LinkedList* stack, Liver value);
Liver pop_stack(LinkedList* stack);
Liver peek_stack(const LinkedList* stack);

void enqueue(LinkedList* queue, Liver value);
Liver dequeue(LinkedList* queue);
Liver peek_queue(const LinkedList* queue);

#endif