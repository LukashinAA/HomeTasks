#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listofwords.h"

/**
 * Создает новый пустой список строк
 * @return Указатель на созданный список или NULL при ошибке
 */
StringList* create_string_list(void) {
    // Выделяем память под структуру списка
    StringList *list = (StringList*)malloc(sizeof(StringList));
    if (list == NULL) {
        return NULL;  // Ошибка выделения памяти
    }
    
    // Инициализируем поля структуры
    list->capacity = 10;  // Начальная вместимость
    list->size = 0;       // Начинаем с пустого списка
    
    // Выделяем память под массив указателей на строки
    list->words = (char**)malloc(list->capacity * sizeof(char*));
    if (list->words == NULL) {
        free(list);  // Освобождаем структуру если не удалось выделить массив
        return NULL;
    }
    
    return list;
}

/**
 * Добавляет строку в список
 * @param list Указатель на список
 * @param str  Строка для добавления (копируется)
 */
void add_string(StringList *list, const char *str) {
    // Проверка валидности параметров
    if (list == NULL || str == NULL) return;
    
    // Проверяем, нужно ли увеличить вместимость массива
    if (list->size >= list->capacity) {
        // Увеличиваем вместимость в 2 раза
        int new_capacity = list->capacity * 2;
        char **new_words = (char**)realloc(list->words, new_capacity * sizeof(char*));
        if (new_words == NULL) return;  // Ошибка перевыделения памяти
        list->words = new_words;
        list->capacity = new_capacity;
    }
    
    // Выделяем память под новую строку
    list->words[list->size] = (char*)malloc(strlen(str) + 1);
    if (list->words[list->size] == NULL) return;  // Ошибка выделения памяти
    
    // Копируем строку в выделенную память
    strcpy(list->words[list->size], str);
    list->size++;  // Увеличиваем счетчик элементов
}

/**
 * Сортирует список строк в лексикографическом порядке (пузырьковая сортировка)
 * @param list Указатель на список для сортировки
 */
void sort_string_list(StringList *list) {
    if (list == NULL || list->size < 2) return;  // Нечего сортировать
    
    // Пузырьковая сортировка
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = 0; j < list->size - i - 1; j++) {
            // Сравниваем соседние строки
            if (strcmp(list->words[j], list->words[j + 1]) > 0) {
                // Меняем указатели местами
                char *temp = list->words[j];
                list->words[j] = list->words[j + 1];
                list->words[j + 1] = temp;
            }
        }
    }
}

/**
 * Выводит список строк на стандартный вывод
 * @param list Указатель на список для вывода
 */
void print_string_list(const StringList *list) {
    if (list == NULL) return;
    
    // Сначала выводим количество строк
    printf("%d\n", list->size);
    
    // Затем выводим каждую строку на отдельной строке
    for (int i = 0; i < list->size; i++) {
        printf("%s\n", list->words[i]);
    }
}

/**
 * Очищает содержимое списка (освобождает строки, но оставляет структуру)
 * @param list Указатель на список для очистки
 */
void clear_string_list(StringList *list) {
    if (list == NULL) return;
    
    // Освобождаем память каждой строки в массиве
    for (int i = 0; i < list->size; i++) {
        free(list->words[i]);
    }
    
    list->size = 0;  // Сбрасываем счетчик элементов
}

/**
 * Полностью уничтожает список, освобождая всю занятую память
 * @param list Указатель на список для уничтожения
 */
void destroy_string_list(StringList *list) {
    if (list == NULL) return;
    
    // Сначала освобождаем все строки
    for (int i = 0; i < list->size; i++) {
        free(list->words[i]);
    }
    
    // Затем освобождаем массив указателей
    free(list->words);
    
    // И наконец освобождаем саму структуру
    free(list);
}