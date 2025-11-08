#ifndef SCANNER_H
#define SCANNER_H

#include "listofwords.h"

// Константы для работы с буферами
#define BLOCK_SIZE 256         // Размер буфера для чтения из входного потока
#define BUFFER_INCREMENT 32    // Шаг увеличения буфера для слова

// Функции для проверки символов
int is_special_char(char c);
int is_word_char(char c);
int is_whitespace(char c);

// Функции для работы с буферами и символами
char getsym(FILE *stream, char *str, int *pos, int *size);
void add_current_word(char **word_buffer, int *word_length, StringList *word_list);
int increase_word_buffer(char **word_buffer, int *word_buffer_size, int *word_length);
void add_char_to_buffer(char c, char **word_buffer, int *word_buffer_size, int *word_length);

// Функция обработки специальных символов
void process_special_char(char c, FILE *stream, char *str, int *pos, int *size, 
                         char **word_buffer, int *word_length, StringList *word_list);

// Основная функция обработки ввода
void process_input(void);

#endif