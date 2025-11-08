#ifndef LISTOFWORDS_H
#define LISTOFWORDS_H

/**
 * Структура для хранения динамического массива строк
 */
typedef struct {
    char **words;      // Указатель на массив указателей на строки
    int capacity;      // Текущая вместимость массива
    int size;          // Количество элементов в массиве
} StringList;

// Функции для работы со списком строк
StringList* create_string_list(void);
void add_string(StringList *list, const char *str);
void sort_string_list(StringList *list);
void print_string_list(const StringList *list);
void clear_string_list(StringList *list);
void destroy_string_list(StringList *list);

#endif