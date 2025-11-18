#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_UNDO 100

UndoAction undo_stack[MAX_UNDO];
int undo_top = -1;
int action_count = 0;

int is_alpha_string(const char* str) {
    if (strlen(str) == 0) {
        return 1;
    }
    for (size_t i = 0; i < strlen(str); i++) {
        if (!isalpha(str[i])) {
            return 0;
        }
    }
    return 1;
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29;
    }
    
    return day <= days_in_month[month - 1];
}

int compare_livers_by_age(const Liver* a, const Liver* b) {
    if (a->birth_date.year != b->birth_date.year) {
        return a->birth_date.year - b->birth_date.year;
    }
    if (a->birth_date.month != b->birth_date.month) {
        return a->birth_date.month - b->birth_date.month;
    }
    return a->birth_date.day - b->birth_date.day;
}

int find_liver_index_by_id(const LinkedList* list, int id) {
    if (list == NULL) {
        return -1;
    }
    
    Node* current = list->head;
    int index = 0;
    while (current != NULL) {
        if (current->data.id == id) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

void insert_sorted_by_age(LinkedList* list, Liver liver) {
    if (list == NULL) {
        return;
    }
    
    if (list->size == 0) {
        push_back_list(list, liver);
        return;
    }
    
    Node* current = list->head;
    size_t index = 0;
    
    while (current != NULL) {
        if (compare_livers_by_age(&liver, &current->data) < 0) {
            break;
        }
        current = current->next;
        index++;
    }
    
    if (current == NULL) {
        push_back_list(list, liver);
    } else {
        insert_at_list(list, index, liver);
    }
}

void save_undo_action(int type, Liver liver, Liver old_liver, int index) {
    if (undo_top >= MAX_UNDO - 1) {
        for (int i = 0; i < MAX_UNDO - 1; i++) {
            undo_stack[i] = undo_stack[i + 1];
        }
        undo_top--;
        if (action_count > 0) action_count--;
    }
    
    undo_top++;
    undo_stack[undo_top].type = type;
    undo_stack[undo_top].liver = liver;
    undo_stack[undo_top].old_liver = old_liver;
    undo_stack[undo_top].index = index;
    
    action_count++;
}

void undo_actions(LinkedList* list) {
    if (undo_top < 0) {
        printf("Нет операций для отмены.\n");
        return;
    }
    
    int undo_count = action_count / 2;
    if (undo_count == 0) undo_count = 1;
    
    printf("Отменяем %d операций...\n", undo_count);
    
    for (int i = 0; i < undo_count && undo_top >= 0; i++) {
        UndoAction action = undo_stack[undo_top];
        undo_top--;
        
        switch (action.type) {
            case 0:
                delete_at_list(list, action.index);
                printf("Отменено добавление жителя ID: %d\n", action.liver.id);
                break;
                
            case 1:
                insert_at_list(list, action.index, action.liver);
                printf("Отменено удаление жителя ID: %d\n", action.liver.id);
                break;
                
            case 2:
                {
                    int current_index = find_liver_index_by_id(list, action.liver.id);
                    if (current_index != -1) {
                        delete_at_list(list, current_index);
                        insert_sorted_by_age(list, action.old_liver);
                        printf("Отменено изменение жителя ID: %d\n", action.liver.id);
                    }
                }
                break;
        }
        action_count--;
    }
}

void print_liver(const Liver* liver) {
    printf("ID: %d, ФИО: %s %s %s, Дата: %02d.%02d.%d, Пол: %c, Доход: %.2f\n",
           liver->id, liver->surname, liver->name, liver->patronymic,
           liver->birth_date.day, liver->birth_date.month, liver->birth_date.year,
           liver->gender, liver->income);
}

void load_from_file(LinkedList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return;
    }
    
    Liver liver;
    int count = 0;
    
    while (fscanf(file, "%d %255s %255s %255s %d %d %d %c %lf",
                  &liver.id, liver.surname, liver.name, liver.patronymic,
                  &liver.birth_date.day, &liver.birth_date.month, &liver.birth_date.year,
                  &liver.gender, &liver.income) == 9) {
        
        if (!is_alpha_string(liver.surname) || !is_alpha_string(liver.name) || 
            !is_alpha_string(liver.patronymic)) {
            printf("Ошибка в данных жителя ID %d\n", liver.id);
            continue;
        }
        
        if (!is_valid_date(liver.birth_date.day, liver.birth_date.month, liver.birth_date.year)) {
            printf("Ошибка в дате жителя ID %d\n", liver.id);
            continue;
        }
        
        if (liver.gender != 'M' && liver.gender != 'W') {
            printf("Ошибка в поле жителя ID %d\n", liver.id);
            continue;
        }
        
        if (liver.income < 0) {
            printf("Ошибка в доходе жителя ID %d\n", liver.id);
            continue;
        }
        
        insert_sorted_by_age(list, liver);
        count++;
    }
    
    fclose(file);
    printf("Загружено %d жителей\n", count);
}

void save_to_file(const LinkedList* list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка создания файла %s\n", filename);
        return;
    }
    
    Node* current = list->head;
    int count = 0;
    
    while (current != NULL) {
        fprintf(file, "%d %s %s %s %d %d %d %c %.2f\n",
                current->data.id, current->data.surname, current->data.name,
                current->data.patronymic, current->data.birth_date.day,
                current->data.birth_date.month, current->data.birth_date.year,
                current->data.gender, current->data.income);
        current = current->next;
        count++;
    }
    
    fclose(file);
    printf("Сохранено %d жителей\n", count);
}

void add_liver(LinkedList* list) {
    Liver liver;
    
    printf("Введите ID: ");
    if (scanf("%d", &liver.id) != 1) {
        printf("Ошибка ввода ID\n");
        return;
    }
    
    if (find_liver_index_by_id(list, liver.id) != -1) {
        printf("Житель с ID %d уже существует\n", liver.id);
        return;
    }
    
    printf("Введите фамилию: ");
    scanf("%255s", liver.surname);
    printf("Введите имя: ");
    scanf("%255s", liver.name);
    printf("Введите отчество: ");
    scanf("%255s", liver.patronymic);
    printf("Введите дату рождения (день месяц год): ");
    scanf("%d %d %d", &liver.birth_date.day, &liver.birth_date.month, &liver.birth_date.year);
    printf("Введите пол (M/W): ");
    scanf(" %c", &liver.gender);
    printf("Введите доход: ");
    scanf("%lf", &liver.income);
    
    if (!is_valid_date(liver.birth_date.day, liver.birth_date.month, liver.birth_date.year)) {
        printf("Неверная дата\n");
        return;
    }
    
    if (liver.gender != 'M' && liver.gender != 'W') {
        printf("Неверный пол\n");
        return;
    }
    
    if (liver.income < 0) {
        printf("Неверный доход\n");
        return;
    }
    
    int index = 0;
    Node* current = list->head;
    while (current != NULL && compare_livers_by_age(&liver, &current->data) >= 0) {
        current = current->next;
        index++;
    }
    
    insert_sorted_by_age(list, liver);
    save_undo_action(0, liver, (Liver){0}, index);
    printf("Житель добавлен\n");
}

void delete_liver(LinkedList* list) {
    int id;
    printf("Введите ID для удаления: ");
    scanf("%d", &id);
    
    int index = find_liver_index_by_id(list, id);
    if (index == -1) {
        printf("Житель не найден\n");
        return;
    }
    
    Liver liver = get_at_list(list, index);
    delete_at_list(list, index);
    save_undo_action(1, liver, (Liver){0}, index);
    printf("Житель удален\n");
}

void update_liver(LinkedList* list) {
    int id;
    printf("Введите ID жителя для изменения: ");
    scanf("%d", &id);
    
    int index = find_liver_index_by_id(list, id);
    if (index == -1) {
        printf("Житель не найден\n");
        return;
    }
    
    Liver old_liver = get_at_list(list, index);
    Liver new_liver = old_liver;
    
    printf("Текущие данные:\n");
    print_liver(&old_liver);
    printf("\n");
    
    printf("Введите новые данные:\n");
    
    printf("Фамилия (%s): ", old_liver.surname);
    scanf("%255s", new_liver.surname);
    if (!is_alpha_string(new_liver.surname)) {
        printf("Фамилия должна содержать только буквы\n");
        return;
    }
    
    printf("Имя (%s): ", old_liver.name);
    scanf("%255s", new_liver.name);
    if (!is_alpha_string(new_liver.name)) {
        printf("Имя должно содержать только буквы\n");
        return;
    }
    
    printf("Отчество (%s): ", old_liver.patronymic);
    scanf("%255s", new_liver.patronymic);
    if (!is_alpha_string(new_liver.patronymic)) {
        printf("Отчество должно содержать только буквы\n");
        return;
    }
    
    printf("Дата рождения (%02d.%02d.%d):\n", 
           old_liver.birth_date.day, old_liver.birth_date.month, old_liver.birth_date.year);
    printf("День: ");
    scanf("%d", &new_liver.birth_date.day);
    printf("Месяц: ");
    scanf("%d", &new_liver.birth_date.month);
    printf("Год: ");
    scanf("%d", &new_liver.birth_date.year);
    
    if (!is_valid_date(new_liver.birth_date.day, new_liver.birth_date.month, new_liver.birth_date.year)) {
        printf("Неверная дата рождения\n");
        return;
    }
    
    printf("Пол (%c): ", old_liver.gender);
    scanf(" %c", &new_liver.gender);
    if (new_liver.gender != 'M' && new_liver.gender != 'W') {
        printf("Пол должен быть M или W\n");
        return;
    }
    
    printf("Доход (%.2f): ", old_liver.income);
    scanf("%lf", &new_liver.income);
    if (new_liver.income < 0) {
        printf("Доход не может быть отрицательным\n");
        return;
    }
    
    delete_at_list(list, index);
    insert_sorted_by_age(list, new_liver);
    
    save_undo_action(2, new_liver, old_liver, index);
    
    printf("Данные жителя обновлены\n");
}

void search_liver(const LinkedList* list) {
    printf("Поиск по:\n1 - ID\n2 - Фамилии\n3 - Диапазону дат\nВыберите: ");
    
    int choice;
    scanf("%d", &choice);
    
    Node* current = list->head;
    int found = 0;
    
    switch (choice) {
        case 1: {
            int id;
            printf("Введите ID: ");
            scanf("%d", &id);
            while (current != NULL) {
                if (current->data.id == id) {
                    print_liver(&current->data);
                    found = 1;
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            char surname[256];
            printf("Введите фамилию: ");
            scanf("%255s", surname);
            while (current != NULL) {
                if (strcmp(current->data.surname, surname) == 0) {
                    print_liver(&current->data);
                    found = 1;
                }
                current = current->next;
            }
            break;
        }
        case 3: {
            int d1, m1, y1, d2, m2, y2;
            printf("Введите начальную дату (день месяц год): ");
            scanf("%d %d %d", &d1, &m1, &y1);
            printf("Введите конечную дату (день месяц год): ");
            scanf("%d %d %d", &d2, &m2, &y2);
            
            while (current != NULL) {
                Liver l = current->data;
                if (compare_livers_by_age(&(Liver){0, "", "", "", {d1, m1, y1}, 0, 0}, &l) <= 0 &&
                    compare_livers_by_age(&l, &(Liver){0, "", "", "", {d2, m2, y2}, 0, 0}) <= 0) {
                    print_liver(&l);
                    found = 1;
                }
                current = current->next;
            }
            break;
        }
        default:
            printf("Неверный выбор\n");
            return;
    }
    
    if (!found) printf("Ничего не найдено\n");
}

void display_all(const LinkedList* list) {
    if (list->size == 0) {
        printf("Список пуст\n");
        return;
    }
    
    printf("\nВсе жители (%zu):\n", list->size);
    Node* current = list->head;
    int i = 1;
    while (current != NULL) {
        printf("%d. ", i++);
        print_liver(&current->data);
        current = current->next;
    }
}

int main() {
    LinkedList list = create_list();
    
    printf("=== Учет жителей ===\n");
    
    int choice;
    char filename[256];
    
    while (1) {
        printf("\n1 - Загрузить из файла\n");
        printf("2 - Сохранить в файл\n");
        printf("3 - Добавить жителя\n");
        printf("4 - Удалить жителя\n");
        printf("5 - Изменить жителя\n");
        printf("6 - Найти жителя\n");
        printf("7 - Показать всех\n");
        printf("8 - Отменить операции\n");
        printf("0 - Выход\n");
        printf("Выбор: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Имя файла: ");
                scanf("%255s", filename);
                load_from_file(&list, filename);
                break;
            case 2:
                printf("Имя файла: ");
                scanf("%255s", filename);
                save_to_file(&list, filename);
                break;
            case 3:
                add_liver(&list);
                break;
            case 4:
                delete_liver(&list);
                break;
            case 5:
                update_liver(&list);
                break;
            case 6:
                search_liver(&list);
                break;
            case 7:
                display_all(&list);
                break;
            case 8:
                undo_actions(&list);
                break;
            case 0:
                delete_list(&list);
                return 0;
            default:
                printf("Неверный выбор\n");
        }
    }
}