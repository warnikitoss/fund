#include "mail_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_menu() {
    printf("\n=== Mail System Menu ===\n");
    printf("1. Add post office\n");
    printf("2. Remove post office\n");
    printf("3. Add letter\n");
    printf("4. Mark letter as undeliverable\n");
    printf("5. Pick up letter\n");
    printf("6. List all letters\n");
    printf("7. Transfer letters (simulate time)\n");
    printf("8. Exit\n");
    printf("Choose an option: ");
}

static void load_mappings(MailSystem *system, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Could not open mappings file: %s\n", filename);
        return;
    }
    
    int id1, id2;
    while (fscanf(file, "%d %d", &id1, &id2) == 2) {
        if (!find_office(system, id1)) {
            int links[] = {id2};
            add_office(system, id1, 10, links, 1);
        }
        if (!find_office(system, id2)) {
            int links[] = {id1};
            add_office(system, id2, 10, links, 1);
        }
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    const char *mappings_file = NULL;
    const char *log_file = NULL;
    
    if (argc > 2) {
        mappings_file = argv[2];
    }
    if (argc > 3) {
        log_file = argv[3];
    }
    
    MailSystem *system = create_mail_system(log_file);
    if (!system) {
        printf("Error: Failed to initialize mail system\n");
        return 1;
    }
    
    if (mappings_file) {
        load_mappings(system, mappings_file);
    }
    
    printf("=== Postal Service Simulation ===\n");
    
    int running = 1;
    while (running) {
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                int id, capacity, num_links;
                printf("Enter office ID: ");
                scanf("%d", &id);
                printf("Enter capacity: ");
                scanf("%d", &capacity);
                printf("Enter number of linked offices: ");
                scanf("%d", &num_links);
                
                int *links = NULL;
                if (num_links > 0) {
                    links = (int*)malloc(num_links * sizeof(int));
                    printf("Enter linked office IDs: ");
                    for (int i = 0; i < num_links; i++) {
                        scanf("%d", &links[i]);
                    }
                }
                
                OperationStatus status = add_office(system, id, capacity, links, num_links);
                free(links);
                
                if (status != SUCCESS) {
                    printf("Error adding office: %d\n", status);
                } else {
                    printf("Office added successfully.\n");
                }
                break;
            }
            
            case 2: {
                int id;
                printf("Enter office ID to remove: ");
                scanf("%d", &id);
                
                OperationStatus status = remove_office(system, id);
                if (status != SUCCESS) {
                    printf("Error removing office: %d\n", status);
                } else {
                    printf("Office removed successfully.\n");
                }
                break;
            }
            
            case 3: {
                int type, priority, from, to;
                char data[256];
                
                printf("Enter letter type (0-Regular, 1-Urgent): ");
                scanf("%d", &type);
                printf("Enter priority: ");
                scanf("%d", &priority);
                printf("Enter from office ID: ");
                scanf("%d", &from);
                printf("Enter to office ID: ");
                scanf("%d", &to);
                printf("Enter technical data: ");
                scanf(" %255[^\n]", data);
                
                OperationStatus status = add_letter(system, (LetterType)type, priority, from, to, data);
                if (status != SUCCESS) {
                    printf("Error adding letter: %d\n", status);
                } else {
                    printf("Letter added successfully.\n");
                }
                break;
            }
            
            case 4: {
                int letter_id;
                printf("Enter letter ID to mark as undeliverable: ");
                scanf("%d", &letter_id);
                
                OperationStatus status = mark_undeliverable(system, letter_id);
                if (status != SUCCESS) {
                    printf("Error marking letter: %d\n", status);
                } else {
                    printf("Letter marked as undeliverable.\n");
                }
                break;
            }
            
            case 5: {
                int letter_id, office_id;
                printf("Enter letter ID: ");
                scanf("%d", &letter_id);
                printf("Enter office ID: ");
                scanf("%d", &office_id);
                
                OperationStatus status = pick_letter(system, letter_id, office_id);
                if (status != SUCCESS) {
                    printf("Error picking up letter: %d\n", status);
                } else {
                    printf("Letter picked up successfully.\n");
                }
                break;
            }
            
            case 6: {
                char filename[256];
                printf("Enter output filename: ");
                scanf(" %255[^\n]", filename);
                
                OperationStatus status = list_letters(system, filename);
                if (status != SUCCESS) {
                    printf("Error listing letters: %d\n", status);
                } else {
                    printf("Letters listed successfully.\n");
                }
                break;
            }
            
            case 7: {
                printf("Transferring letters...\n");
                transfer_letters(system);
                printf("Letters transferred.\n");
                break;
            }
            
            case 8:
                running = 0;
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
    
    destroy_mail_system(system);
    return 0;
}