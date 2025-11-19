#include "mail_system.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_create_destroy_mail_system() {
    printf("Testing mail system creation/destruction... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    assert(system != NULL);
    assert(system->offices == NULL);
    assert(system->letters_size == 0);
    assert(system->letters_capacity == 0);
    assert(system->next_letter_id == 1);
    assert(system->log_file != NULL);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_add_office() {
    printf("Testing office addition... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    int links[] = {2, 3};
    OperationStatus status = add_office(system, 1, 10, links, 2);
    assert(status == SUCCESS);
    
    PostOffice* office = find_office(system, 1);
    assert(office != NULL);
    assert(office->id == 1);
    assert(office->capacity == 10);
    assert(office->current_letters == 0);
    assert(office->num_links == 2);
    assert(office->linked_offices[0] == 2);
    assert(office->linked_offices[1] == 3);
    
    status = add_office(system, 1, 5, NULL, 0);
    assert(status == ERROR_DUPLICATE_OFFICE);
    
    status = add_office(system, -1, 10, NULL, 0);
    assert(status == ERROR_INVALID_ID);
    
    status = add_office(system, 2, 0, NULL, 0);
    assert(status == ERROR_INVALID_ID);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_add_letter() {
    printf("Testing letter addition... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    add_office(system, 1, 10, NULL, 0);
    add_office(system, 2, 10, NULL, 0);
    
    OperationStatus status = add_letter(system, REGULAR, 1, 1, 2, "test_data");
    assert(status == SUCCESS);
    assert(system->letters_size == 1);
    
    Letter* letter = find_letter(system, 1);
    assert(letter != NULL);
    assert(letter->id == 1);
    assert(letter->type == REGULAR);
    assert(letter->status == IN_TRANSIT);
    assert(letter->priority == 1);
    assert(letter->from_office == 1);
    assert(letter->to_office == 2);
    assert(strcmp(letter->technical_data, "test_data") == 0);
    
    status = add_letter(system, REGULAR, -1, 1, 2, "test_data");
    assert(status == ERROR_INVALID_PARAMETER);
    
    status = add_letter(system, REGULAR, 1, 3, 2, "test_data");
    assert(status == ERROR_OFFICE_NOT_FOUND);
    
    status = add_letter(system, REGULAR, 1, 1, 3, "test_data");
    assert(status == ERROR_OFFICE_NOT_FOUND);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_heap_operations() {
    printf("Testing heap operations... ");
    
    Heap h = create_heap(5);
    assert(h.data != NULL);
    assert(h.size == 0);
    assert(h.capacity == 5);
    
    push_heap(&h, 5);
    push_heap(&h, 3);
    push_heap(&h, 7);
    push_heap(&h, 1);
    push_heap(&h, 4);
    
    assert(h.size == 5);
    assert(peek_heap(&h) == 1);
    
    assert(pop_heap(&h) == 1);
    assert(pop_heap(&h) == 3);
    assert(pop_heap(&h) == 4);
    assert(pop_heap(&h) == 5);
    assert(pop_heap(&h) == 7);
    assert(is_empty_heap(&h));
    
    int arr[] = {9, 5, 6, 2, 3};
    Heap h2 = build_heap(arr, 5);
    assert(h2.size == 5);
    assert(peek_heap(&h2) == 2);
    
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2, 4};
    
    Heap h3 = build_heap(arr1, 3);
    Heap h4 = build_heap(arr2, 3);
    Heap h5 = build_heap(arr3, 3);
    
    assert(is_equal_heap(&h3, &h4) == 1);
    assert(is_equal_heap(&h3, &h5) == 0);
    
    delete_heap(&h);
    delete_heap(&h2);
    delete_heap(&h3);
    delete_heap(&h4);
    delete_heap(&h5);
    printf("PASSED\n");
}

void test_letter_transfer() {
    printf("Testing letter transfer... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    int links1[] = {2};
    int links2[] = {1, 3};
    int links3[] = {2};
    
    add_office(system, 1, 10, links1, 1);
    add_office(system, 2, 10, links2, 2);
    add_office(system, 3, 10, links3, 1);
    
    add_letter(system, REGULAR, 1, 1, 3, "transfer_test");

    PostOffice* office1 = find_office(system, 1);
    assert(office1->current_letters == 1);
    assert(!is_empty_heap(&office1->letter_heap));
    
    transfer_letters(system);
    
    office1 = find_office(system, 1);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_mark_undeliverable() {
    printf("Testing mark undeliverable... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    add_office(system, 1, 10, NULL, 0);
    add_office(system, 2, 10, NULL, 0);
    add_letter(system, REGULAR, 1, 1, 2, "test_data");
    
    Letter* letter = find_letter(system, 1);
    assert(letter->status == IN_TRANSIT);
    
    OperationStatus status = mark_undeliverable(system, 1);
    assert(status == SUCCESS);
    
    letter = find_letter(system, 1);
    assert(letter->status == UNDELIVERABLE);
    
    status = mark_undeliverable(system, 999);
    assert(status == ERROR_LETTER_NOT_FOUND);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_office_removal() {
    printf("Testing office removal... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    int links[] = {2};
    add_office(system, 1, 10, links, 1);
    add_office(system, 2, 10, NULL, 0);
    add_letter(system, REGULAR, 1, 1, 2, "test_data");
    
    assert(find_office(system, 1) != NULL);
    
    OperationStatus status = remove_office(system, 1);
    assert(status == SUCCESS);
    assert(find_office(system, 1) == NULL);
    
    Letter* letter = find_letter(system, 1);
    assert(letter != NULL);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void test_letter_pickup() {
    printf("Testing letter pickup... ");
    
    MailSystem* system = create_mail_system("test_log.txt");
    
    add_office(system, 1, 10, NULL, 0);
    add_office(system, 2, 10, NULL, 0);
    add_letter(system, REGULAR, 1, 1, 2, "test_data");
    
    Letter* letter = find_letter(system, 1);
    letter->status = DELIVERED;
    
    PostOffice* office2 = find_office(system, 2);
    push_heap(&office2->letter_heap, 1);
    office2->current_letters++;
    
    OperationStatus status = pick_letter(system, 1, 2);
    assert(status == SUCCESS);
    
    letter = find_letter(system, 1);
    assert(letter == NULL);
    
    destroy_mail_system(system);
    printf("PASSED\n");
}

void run_all_tests() {
    printf("=== Starting Mail System Tests ===\n\n");
    
    test_create_destroy_mail_system();
    test_heap_operations();
    test_add_office();
    test_add_letter();
    test_letter_transfer();
    test_mark_undeliverable();
    test_office_removal();
    test_letter_pickup();
    
    printf("\n=== All tests passed! ===\n");
}

int main() {
    run_all_tests();
    return 0;
}