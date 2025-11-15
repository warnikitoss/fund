#include "functions.h"
#include <stdlib.h>
#include <string.h>

LinkedList create_list(void) {
    LinkedList list;
    list.head = NULL;
    list.tail = NULL;
    list.size = 0;
    return list;
}

void erase_list(LinkedList* list) {
    if (list == NULL) {
        return;
    }
    
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void delete_list(LinkedList* list) {
    erase_list(list);
}

void push_back_list(LinkedList* list, Liver value) {
    if (list == NULL) {
        return;
    }
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    
    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    list->size++;
}

void push_front_list(LinkedList* list, Liver value) {
    if (list == NULL) {
        return;
    }
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    
    new_node->data = value;
    new_node->prev = NULL;
    new_node->next = list->head;
    
    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    list->head = new_node;
    list->size++;
}

Liver pop_back_list(LinkedList* list) {
    Liver result = {0};
    if (list == NULL || list->tail == NULL) {
        return result;
    }
    
    Node* last = list->tail;
    result = last->data;
    
    if (last->prev != NULL) {
        last->prev->next = NULL;
    } else {
        list->head = NULL;
    }
    
    list->tail = last->prev;
    free(last);
    list->size--;
    
    return result;
}

Liver pop_front_list(LinkedList* list) {
    Liver result = {0};
    if (list == NULL || list->head == NULL) {
        return result;
    }
    
    Node* first = list->head;
    result = first->data;
    
    if (first->next != NULL) {
        first->next->prev = NULL;
    } else {
        list->tail = NULL;
    }
    
    list->head = first->next;
    free(first);
    list->size--;
    
    return result;
}

void insert_at_list(LinkedList* list, size_t index, Liver value) {
    if (list == NULL || index > list->size) {
        return;
    }
    
    if (index == 0) {
        push_front_list(list, value);
        return;
    }
    
    if (index == list->size) {
        push_back_list(list, value);
        return;
    }
    
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    
    new_node->data = value;
    new_node->prev = current->prev;
    new_node->next = current;
    
    current->prev->next = new_node;
    current->prev = new_node;
    list->size++;
}

void delete_at_list(LinkedList* list, size_t index) {
    if (list == NULL || index >= list->size) {
        return;
    }
    
    if (index == 0) {
        pop_front_list(list);
        return;
    }
    
    if (index == list->size - 1) {
        pop_back_list(list);
        return;
    }
    
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    list->size--;
}

Liver get_at_list(const LinkedList* list, size_t index) {
    Liver result = {0};
    if (list == NULL || index >= list->size) {
        return result;
    }
    
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->data;
}

int is_equal_list(const LinkedList* l1, const LinkedList* l2) {
    if (l1 == NULL && l2 == NULL) {
        return 1;
    }
    if (l1 == NULL || l2 == NULL) {
        return 0;
    }
    if (l1->size != l2->size) {
        return 0;
    }
    
    Node* curr1 = l1->head;
    Node* curr2 = l2->head;
    
    while (curr1 != NULL && curr2 != NULL) {
        if (memcmp(&curr1->data, &curr2->data, sizeof(Liver)) != 0) {
            return 0;
        }
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    
    return 1;
}

void push_stack(LinkedList* stack, Liver value) {
    push_back_list(stack, value);
}

Liver pop_stack(LinkedList* stack) {
    return pop_back_list(stack);
}

Liver peek_stack(const LinkedList* stack) {
    if (stack == NULL || stack->tail == NULL) {
        Liver empty = {0};
        return empty;
    }
    return stack->tail->data;
}

void enqueue(LinkedList* queue, Liver value) {
    push_back_list(queue, value);
}

Liver dequeue(LinkedList* queue) {
    return pop_front_list(queue);
}

Liver peek_queue(const LinkedList* queue) {
    if (queue == NULL || queue->head == NULL) {
        Liver empty = {0};
        return empty;
    }
    return queue->head->data;
}