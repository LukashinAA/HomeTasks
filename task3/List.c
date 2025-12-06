#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

// Начальная емкость списка
#define INITIAL_CAPACITY 10


// Создание нового списка
StringList* create_list() {
    StringList *list = (StringList*)malloc(sizeof(StringList));
    if (!list) {
        return NULL;
    }
    
    list->items = (char**)malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!list->items) {
        free(list);
        return NULL;
    }
    
    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    return list;
}

// Добавление строки в список
void add_string(StringList *list, const char *str) {
    if (list->size >= list->capacity) {
        // Увеличиваем емкость
        int new_capacity = list->capacity * 2;
        char **new_items = (char**)realloc(list->items, new_capacity * sizeof(char*));
        if (!new_items) {
            return; // Не удалось увеличить
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    
    // Копируем строку вручную
    list->items[list->size] = strdup(str);
    if (list->items[list->size]) {
        list->size++;
    }
}

// Сортировка списка (пузырьковая сортировка)
void sort_list(StringList *list) {
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = 0; j < list->size - i - 1; j++) {
            if (strcmp(list->items[j], list->items[j + 1]) > 0) {
                // Меняем местами
                char *temp = list->items[j];
                list->items[j] = list->items[j + 1];
                list->items[j + 1] = temp;
            }
        }
    }
}

// Вывод списка
void print_list(StringList *list) {
    printf("%d\n", list->size);
    for (int i = 0; i < list->size; i++) {
        printf("%s\n", list->items[i]);
    }
}

// Освобождение памяти
void free_list(StringList *list) {
    for (int i = 0; i < list->size; i++) {
        free(list->items[i]);
    }
    free(list->items);
    free(list);
}