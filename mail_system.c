#include "mail_system.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

static void log_event(MailSystem *system, const char *message) {
    if (!system || !system->log_file) {
        return;
    }
    
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(system->log_file, "[%s] %s\n", time_str, message);
    fflush(system->log_file);
}

static OperationStatus transfer_letter_to_office(MailSystem *system, int letter_id, int from_office_id, int to_office_id) {
    if (!system) {
        return ERROR_INVALID_PARAMETER;
    }
    
    PostOffice *target_office = find_office(system, to_office_id);
    if (!target_office) {
        return ERROR_OFFICE_NOT_FOUND;
    }
    
    if (target_office->current_letters >= target_office->capacity) {
        return ERROR_OFFICE_FULL;
    }
    
    push_heap(&target_office->letter_heap, letter_id);
    target_office->current_letters++;
    
    char log_msg[256];
    sprintf(log_msg, "Letter %d transferred from office %d to office %d", 
            letter_id, from_office_id, to_office_id);
    log_event(system, log_msg);
    
    return SUCCESS;
}

MailSystem* create_mail_system(const char *log_filename) {
    MailSystem *system = (MailSystem*)malloc(sizeof(MailSystem));
    if (!system) {
        return NULL;
    }
    
    system->offices = NULL;
    system->letters = NULL;
    system->letters_size = 0;
    system->letters_capacity = 0;
    system->next_letter_id = 1;
    
    if (log_filename) {
        system->log_file = fopen(log_filename, "w");
    } else {
        system->log_file = fopen("mail_log.txt", "w");
    }
    
    if (!system->log_file) {
        free(system);
        return NULL;
    }
    
    log_event(system, "Mail system initialized");
    return system;
}

void destroy_mail_system(MailSystem *system) {
    if (!system) {
        return;
    }
    
    PostOffice *current = system->offices;
    while (current) {
        PostOffice *next = current->next;
        delete_heap(&current->letter_heap);
        free(current->linked_offices);
        free(current);
        current = next;
    }
    
    free(system->letters);
    
    if (system->log_file) {
        fclose(system->log_file);
    }
    
    free(system);
}

PostOffice* find_office(const MailSystem *system, int office_id) {
    if (!system) {
        return NULL;
    }
    
    PostOffice *current = system->offices;
    while (current) {
        if (current->id == office_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Letter* find_letter(MailSystem *system, int letter_id) {
    if (!system) {
        return NULL;
    }
    
    for (size_t i = 0; i < system->letters_size; i++) {
        if (system->letters[i].id == letter_id) {
            return &system->letters[i];
        }
    }
    return NULL;
}

OperationStatus add_office(MailSystem *system, int id, int capacity, const int *linked_offices, int num_links) {
    if (!system || id < 0 || capacity <= 0) {
        return ERROR_INVALID_ID;
    }
    
    if (find_office(system, id)) {
        return ERROR_DUPLICATE_OFFICE;
    }
    
    PostOffice *new_office = (PostOffice*)malloc(sizeof(PostOffice));
    if (!new_office) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    new_office->id = id;
    new_office->capacity = capacity;
    new_office->current_letters = 0;
    new_office->num_links = num_links;
    new_office->letter_heap = create_heap(10);
    new_office->next = system->offices;
    system->offices = new_office;
    
    if (num_links > 0) {
        new_office->linked_offices = (int*)malloc(num_links * sizeof(int));
        if (!new_office->linked_offices) {
            delete_heap(&new_office->letter_heap);
            free(new_office);
            return ERROR_MEMORY_ALLOCATION;
        }
        memcpy(new_office->linked_offices, linked_offices, num_links * sizeof(int));
    } else {
        new_office->linked_offices = NULL;
    }
    
    char log_msg[256];
    sprintf(log_msg, "Added office %d with capacity %d", id, capacity);
    log_event(system, log_msg);
    
    return SUCCESS;
}

OperationStatus remove_office(MailSystem *system, int id) {
    if (!system) {
        return ERROR_INVALID_ID;
    }
    
    PostOffice **prev = &system->offices;
    PostOffice *current = system->offices;
    
    while (current) {
        if (current->id == id) {
            while (!is_empty_heap(&current->letter_heap)) {
                int letter_id = pop_heap(&current->letter_heap);
                Letter *letter = find_letter(system, letter_id);
                if (letter) {
                    if (letter->from_office == id || letter->to_office == id) {
                        letter->status = UNDELIVERABLE;
                        char log_msg[256];
                        sprintf(log_msg, "Letter %d marked as undeliverable (office %d removed)", 
                                letter_id, id);
                        log_event(system, log_msg);
                    } else {
                        if (current->num_links > 0) {
                            int attempts = current->num_links;
                            int start_index = rand() % current->num_links;
                            
                            for (int i = 0; i < attempts; i++) {
                                int target_index = (start_index + i) % current->num_links;
                                int target_id = current->linked_offices[target_index];
                                
                                if (transfer_letter_to_office(system, letter_id, id, target_id) == SUCCESS) {
                                    break;
                                }
                            }
                        } else {
                            letter->status = UNDELIVERABLE;
                            char log_msg[256];
                            sprintf(log_msg, "Letter %d marked as undeliverable (no route after office removal)", 
                                    letter_id);
                            log_event(system, log_msg);
                        }
                    }
                }
            }
            PostOffice *other_office = system->offices;
            while (other_office) {
                if (other_office->id != id) {
                    for (int i = 0; i < other_office->num_links; i++) {
                        if (other_office->linked_offices[i] == id) {
                            for (int j = i; j < other_office->num_links - 1; j++) {
                                other_office->linked_offices[j] = other_office->linked_offices[j + 1];
                            }
                            other_office->num_links--;
                            
                            if (other_office->num_links > 0) {
                                int *new_links = (int*)realloc(other_office->linked_offices, 
                                                              other_office->num_links * sizeof(int));
                                if (new_links) {
                                    other_office->linked_offices = new_links;
                                }
                            } else {
                                free(other_office->linked_offices);
                                other_office->linked_offices = NULL;
                            }
                            break;
                        }
                    }
                }
                other_office = other_office->next;
            }
            
            *prev = current->next;
            delete_heap(&current->letter_heap);
            free(current->linked_offices);
            free(current);
            
            char log_msg[256];
            sprintf(log_msg, "Removed office %d", id);
            log_event(system, log_msg);
            
            return SUCCESS;
        }
        prev = &current->next;
        current = current->next;
    }
    
    return ERROR_OFFICE_NOT_FOUND;
}

OperationStatus add_letter(MailSystem *system, LetterType type, int priority, int from_office, int to_office, const char *technical_data) {
    if (!system || priority < 0 || !technical_data) {
        return ERROR_INVALID_PARAMETER;
    }
    
    if (!find_office(system, from_office) || !find_office(system, to_office)) {
        return ERROR_OFFICE_NOT_FOUND;
    }
    
    if (system->letters_size >= system->letters_capacity) {
        size_t new_capacity = system->letters_capacity == 0 ? 10 : system->letters_capacity * 2;
        Letter *new_letters = (Letter*)realloc(system->letters, new_capacity * sizeof(Letter));
        if (!new_letters) {
            return ERROR_MEMORY_ALLOCATION;
        }
        system->letters = new_letters;
        system->letters_capacity = new_capacity;
    }
    
    Letter *new_letter = &system->letters[system->letters_size];
    new_letter->id = system->next_letter_id++;
    new_letter->type = type;
    new_letter->status = IN_TRANSIT;
    new_letter->priority = priority;
    new_letter->from_office = from_office;
    new_letter->to_office = to_office;
    strncpy(new_letter->technical_data, technical_data, sizeof(new_letter->technical_data) - 1);
    new_letter->technical_data[sizeof(new_letter->technical_data) - 1] = '\0';
    
    PostOffice *office = find_office(system, from_office);
    if (office->current_letters >= office->capacity) {
        return ERROR_OFFICE_FULL;
    }
    
    push_heap(&office->letter_heap, new_letter->id);
    office->current_letters++;
    system->letters_size++;
    
    char log_msg[256];
    sprintf(log_msg, "Added letter %d from office %d to office %d", new_letter->id, from_office, to_office);
    log_event(system, log_msg);
    
    return SUCCESS;
}

OperationStatus mark_undeliverable(MailSystem *system, int letter_id) {
    if (!system) {
        return ERROR_INVALID_ID;
    }
    
    Letter *letter = find_letter(system, letter_id);
    if (!letter) {
        return ERROR_LETTER_NOT_FOUND;
    }
    
    letter->status = UNDELIVERABLE;
    
    PostOffice *office = system->offices;
    while (office) {
        Heap temp_heap = create_heap(office->letter_heap.capacity);
        int found = 0;
        
        while (!is_empty_heap(&office->letter_heap)) {
            int current_id = pop_heap(&office->letter_heap);
            if (current_id != letter_id) {
                push_heap(&temp_heap, current_id);
            } else {
                found = 1;
                office->current_letters--;
            }
        }
        
        delete_heap(&office->letter_heap);
        office->letter_heap = temp_heap;
        
        if (found) {
            break;
        }
        office = office->next;
    }
    
    char log_msg[256];
    sprintf(log_msg, "Letter %d marked as undeliverable", letter_id);
    log_event(system, log_msg);
    
    return SUCCESS;
}

OperationStatus pick_letter(MailSystem *system, int letter_id, int office_id) {
    if (!system) {
        return ERROR_INVALID_ID;
    }
    
    PostOffice *office = find_office(system, office_id);
    if (!office) {
        return ERROR_OFFICE_NOT_FOUND;
    }
    
    Letter *letter = find_letter(system, letter_id);
    if (!letter) {
        return ERROR_LETTER_NOT_FOUND;
    }
    
    if (letter->to_office != office_id) {
        return ERROR_LETTER_NOT_FOUND;
    }
    
    if (letter->status != DELIVERED) {
        return ERROR_LETTER_NOT_FOUND;
    }
    
    Heap temp_heap = create_heap(office->letter_heap.capacity);
    int found = 0;
    
    while (!is_empty_heap(&office->letter_heap)) {
        int current_id = pop_heap(&office->letter_heap);
        if (current_id != letter_id) {
            push_heap(&temp_heap, current_id);
        } else {
            found = 1;
            office->current_letters--;
        }
    }
    
    delete_heap(&office->letter_heap);
    office->letter_heap = temp_heap;
    
    if (!found) {
        return ERROR_LETTER_NOT_FOUND;
    }
    
    for (size_t i = 0; i < system->letters_size; i++) {
        if (system->letters[i].id == letter_id) {
            for (size_t j = i; j < system->letters_size - 1; j++) {
                system->letters[j] = system->letters[j + 1];
            }
            system->letters_size--;
            
            char log_msg[256];
            sprintf(log_msg, "Letter %d picked up from office %d", letter_id, office_id);
            log_event(system, log_msg);
            
            return SUCCESS;
        }
    }
    
    return ERROR_LETTER_NOT_FOUND;
}

OperationStatus list_letters(const MailSystem *system, const char *filename) {
    if (!system || !filename) {
        return ERROR_INVALID_ID;
    }
    
    FILE *out = fopen(filename, "w");
    if (!out) {
        return ERROR_FILE_OPERATION;
    }
    
    fprintf(out, "Total letters: %zu\n", system->letters_size);
    for (size_t i = 0; i < system->letters_size; i++) {
        const Letter *l = &system->letters[i];
        fprintf(out, "Letter ID: %d, Type: %s, Status: %s, Priority: %d, From: %d, To: %d, Data: %s\n",
                l->id,
                l->type == REGULAR ? "Regular" : "Urgent",
                l->status == IN_TRANSIT ? "In Transit" : 
                 (l->status == DELIVERED ? "Delivered" : "Undeliverable"),
                l->priority,
                l->from_office,
                l->to_office,
                l->technical_data);
    }
    fclose(out);
    
    return SUCCESS;
}

void transfer_letters(MailSystem *system) {
    if (!system) {
        return;
    }
    
    PostOffice *office = system->offices;
    while (office) {
        if (!is_empty_heap(&office->letter_heap)) {
            Heap temp_heap = create_heap(office->letter_heap.capacity);
            int processed_count = 0;
            const int max_processing = office->current_letters;
            
            while (!is_empty_heap(&office->letter_heap) && processed_count < max_processing) {
                int letter_id = pop_heap(&office->letter_heap);
                office->current_letters--;
                processed_count++;
                
                Letter *letter = find_letter(system, letter_id);
                if (!letter) {
                    continue;
                }
                
                if (letter->to_office == office->id) {
                    letter->status = DELIVERED;
                    push_heap(&temp_heap, letter_id);
                    office->current_letters++;
                    
                    char log_msg[256];
                    sprintf(log_msg, "Letter %d delivered to office %d", letter_id, office->id);
                    log_event(system, log_msg);
                } else if (office->num_links > 0) {
                    int transferred = 0;
                    
                    int attempts = office->num_links;
                    int start_index = rand() % office->num_links;
                    
                    for (int i = 0; i < attempts && !transferred; i++) {
                        int target_index = (start_index + i) % office->num_links;
                        int target_id = office->linked_offices[target_index];
                        
                        if (transfer_letter_to_office(system, letter_id, office->id, target_id) == SUCCESS) {
                            transferred = 1;
                        }
                    }
                    
                    if (!transferred) {
                        push_heap(&temp_heap, letter_id);
                        office->current_letters++;
                    }
                } else {
                    letter->status = UNDELIVERABLE;
                    
                    char log_msg[256];
                    sprintf(log_msg, "Letter %d marked as undeliverable (no route from office %d)", 
                            letter_id, office->id);
                    log_event(system, log_msg);
                }
            }
  
            while (!is_empty_heap(&office->letter_heap)) {
                push_heap(&temp_heap, pop_heap(&office->letter_heap));
            }
            
            delete_heap(&office->letter_heap);
            office->letter_heap = temp_heap;
        }
        office = office->next;
    }
}