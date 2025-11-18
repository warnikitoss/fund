#ifndef MAIL_SYSTEM_H
#define MAIL_SYSTEM_H

#include "heap.h"
#include <stddef.h>
#include <stdio.h>

typedef enum {
    REGULAR,
    URGENT
} LetterType;

typedef enum {
    IN_TRANSIT,
    DELIVERED,
    UNDELIVERABLE
} LetterStatus;

typedef struct {
    int id;
    LetterType type;
    LetterStatus status;
    int priority;
    int from_office;
    int to_office;
    char technical_data[256];
} Letter;

typedef struct PostOffice {
    int id;
    int capacity;
    int current_letters;
    int *linked_offices;
    int num_links;
    Heap letter_heap;
    struct PostOffice *next;
} PostOffice;

typedef struct {
    PostOffice *offices;
    Letter *letters;
    size_t letters_size;
    size_t letters_capacity;
    int next_letter_id;
    FILE *log_file;
} MailSystem;

typedef enum {
    SUCCESS,
    ERROR_OFFICE_NOT_FOUND,
    ERROR_OFFICE_FULL,
    ERROR_LETTER_NOT_FOUND,
    ERROR_INVALID_ID,
    ERROR_MEMORY_ALLOCATION,
    ERROR_FILE_OPERATION,
    ERROR_DUPLICATE_OFFICE,
    ERROR_INVALID_PRIORITY,
    ERROR_INVALID_PARAMETER
} OperationStatus;

MailSystem* create_mail_system(const char *log_filename);
void destroy_mail_system(MailSystem *system);

OperationStatus add_office(MailSystem *system, int id, int capacity, const int *linked_offices, int num_links);
OperationStatus remove_office(MailSystem *system, int id);
OperationStatus add_letter(MailSystem *system, LetterType type, int priority, int from_office, int to_office, const char *technical_data);
OperationStatus mark_undeliverable(MailSystem *system, int letter_id);
OperationStatus pick_letter(MailSystem *system, int letter_id, int office_id);
OperationStatus list_letters(const MailSystem *system, const char *filename);
void transfer_letters(MailSystem *system);

PostOffice* find_office(const MailSystem *system, int office_id);
Letter* find_letter(MailSystem *system, int letter_id);

#endif