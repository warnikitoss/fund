#include "functions.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_create_list() {
    LinkedList list = create_list();
    assert(list.head == NULL);
    assert(list.tail == NULL);
    assert(list.size == 0);
    printf("test_create_list passed\n");
}

void test_push_back_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    push_back_list(&list, liver1);
    assert(list.size == 1);
    assert(list.head != NULL);
    assert(list.tail != NULL);
    assert(list.head == list.tail);
    assert(list.head->data.id == 1);

    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    push_back_list(&list, liver2);
    assert(list.size == 2);
    assert(list.head->data.id == 1);
    assert(list.tail->data.id == 2);
    assert(list.head->next == list.tail);
    assert(list.tail->prev == list.head);
    
    delete_list(&list);
    printf("test_push_back_list passed\n");
}

void test_push_front_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    push_front_list(&list, liver1);
    assert(list.size == 1);
    assert(list.head != NULL);
    assert(list.tail != NULL);
    assert(list.head == list.tail);
    assert(list.head->data.id == 1);

    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    push_front_list(&list, liver2);
    assert(list.size == 2);
    assert(list.head->data.id == 2);
    assert(list.tail->data.id == 1);
    assert(list.head->next == list.tail);
    assert(list.tail->prev == list.head);
    
    delete_list(&list);
    printf("test_push_front_list passed\n");
}

void test_pop_back_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    push_back_list(&list, liver1);
    push_back_list(&list, liver2);

    Liver popped = pop_back_list(&list);
    assert(popped.id == 2);
    assert(list.size == 1);
    assert(list.head->data.id == 1);
    assert(list.tail->data.id == 1);

    popped = pop_back_list(&list);
    assert(popped.id == 1);
    assert(list.size == 0);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    popped = pop_back_list(&list);
    assert(popped.id == 0);
    
    delete_list(&list);
    printf("test_pop_back_list passed\n");
}

void test_pop_front_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    push_back_list(&list, liver1);
    push_back_list(&list, liver2);

    Liver popped = pop_front_list(&list);
    assert(popped.id == 1);
    assert(list.size == 1);
    assert(list.head->data.id == 2);
    assert(list.tail->data.id == 2);

    popped = pop_front_list(&list);
    assert(popped.id == 2);
    assert(list.size == 0);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    popped = pop_front_list(&list);
    assert(popped.id == 0);
    
    delete_list(&list);
    printf("test_pop_front_list passed\n");
}

void test_insert_at_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    Liver liver3 = {3, "Sidorov", "Sidor", "Sidorovich", {3, 3, 1992}, 'M', 3000.0};

    insert_at_list(&list, 0, liver1);
    assert(list.size == 1);
    assert(list.head->data.id == 1);

    insert_at_list(&list, 0, liver2);
    assert(list.size == 2);
    assert(list.head->data.id == 2);
    assert(list.tail->data.id == 1);

    insert_at_list(&list, 2, liver3);
    assert(list.size == 3);
    assert(list.tail->data.id == 3);

    Liver liver4 = {4, "Fedorov", "Fedor", "Fedorovich", {4, 4, 1993}, 'M', 4000.0};
    insert_at_list(&list, 1, liver4);
    assert(list.size == 4);
    assert(list.head->next->data.id == 4);

    delete_list(&list);
    printf("test_insert_at_list passed\n");
}

void test_delete_at_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    Liver liver3 = {3, "Sidorov", "Sidor", "Sidorovich", {3, 3, 1992}, 'M', 3000.0};
    push_back_list(&list, liver1);
    push_back_list(&list, liver2);
    push_back_list(&list, liver3);

    delete_at_list(&list, 1);
    assert(list.size == 2);
    assert(list.head->data.id == 1);
    assert(list.tail->data.id == 3);

    delete_at_list(&list, 0);
    assert(list.size == 1);
    assert(list.head->data.id == 3);

    delete_at_list(&list, 0);
    assert(list.size == 0);
    assert(list.head == NULL);
    assert(list.tail == NULL);

    delete_list(&list);
    printf("test_delete_at_list passed\n");
}

void test_get_at_list() {
    LinkedList list = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};
    push_back_list(&list, liver1);
    push_back_list(&list, liver2);

    Liver got = get_at_list(&list, 0);
    assert(got.id == 1);

    got = get_at_list(&list, 1);
    assert(got.id == 2);

    got = get_at_list(&list, 2);
    assert(got.id == 0);

    delete_list(&list);
    printf("test_get_at_list passed\n");
}

void test_is_equal_list() {
    LinkedList list1 = create_list();
    LinkedList list2 = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};

    push_back_list(&list1, liver1);
    push_back_list(&list1, liver2);
    push_back_list(&list2, liver1);
    push_back_list(&list2, liver2);

    assert(is_equal_list(&list1, &list2) == 1);

    list2.head->data.id = 3;
    assert(is_equal_list(&list1, &list2) == 0);

    delete_list(&list1);
    delete_list(&list2);
    printf("test_is_equal_list passed\n");
}

void test_stack_operations() {
    LinkedList stack = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};

    push_stack(&stack, liver1);
    push_stack(&stack, liver2);
    assert(stack.size == 2);

    Liver popped = pop_stack(&stack);
    assert(popped.id == 2);
    assert(stack.size == 1);

    popped = pop_stack(&stack);
    assert(popped.id == 1);
    assert(stack.size == 0);

    delete_list(&stack);
    printf("test_stack_operations passed\n");
}

void test_queue_operations() {
    LinkedList queue = create_list();
    Liver liver1 = {1, "Ivanov", "Ivan", "Ivanovich", {1, 1, 1990}, 'M', 1000.0};
    Liver liver2 = {2, "Petrov", "Petr", "Petrovich", {2, 2, 1991}, 'M', 2000.0};

    enqueue(&queue, liver1);
    enqueue(&queue, liver2);
    assert(queue.size == 2);

    Liver dequeued = dequeue(&queue);
    assert(dequeued.id == 1);
    assert(queue.size == 1);

    dequeued = dequeue(&queue);
    assert(dequeued.id == 2);
    assert(queue.size == 0);

    delete_list(&queue);
    printf("test_queue_operations passed\n");
}

void run_all_tests() {
    test_create_list();
    test_push_back_list();
    test_push_front_list();
    test_pop_back_list();
    test_pop_front_list();
    test_insert_at_list();
    test_delete_at_list();
    test_get_at_list();
    test_is_equal_list();
    test_stack_operations();
    test_queue_operations();
}

int main() {
    run_all_tests();
    printf("All tests passed!\n");
    return 0;
}