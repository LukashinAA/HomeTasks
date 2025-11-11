#ifndef LIST_H
#define LIST_H

// Структура для списка строк
typedef struct {
    char **items;      // массив указателей на строки
    int capacity;      // текущая емкость массива
    int size;          // текущее количество элементов
} StringList;

// Функции для работы со списком
StringList* create_list();
void add_string(StringList *list, const char *str);
void sort_list(StringList *list);
void print_list(StringList *list);
void free_list(StringList *list);

#endif
