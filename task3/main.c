#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "List.h"

#define BUFFER_SIZE 256
#define WORD_BUF_INIT_SIZE 16

// Специальные слова
const char *SPECIAL_WORDS[] = {"||", ">>", "&&", "|", ">", "<", ";", "&", "(", ")"};
const int SPECIAL_COUNT = 10;

// Глобальные переменные для getsym
static char input_buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int buffer_size = 0;
static int eof_reached = 0;

// Функция getsym - получает следующий символ с использованием fscanf
int getsym(FILE *stream) {
    if (buffer_pos >= buffer_size) {
        // Буфер исчерпан, читаем новую порцию с помощью fscanf
        if (fscanf(stream, "%255c", input_buffer) == 1) {
            buffer_size = strlen(input_buffer);
            buffer_pos = 0;
        } else {
            eof_reached = 1;
            return EOF;
        }
    }
    
    return input_buffer[buffer_pos++];
}

// Проверка, является ли символ специальным
int is_special_char(int c) {
    return c == '|' || c == '&' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')';
}

// Проверка, является ли строка специальным словом
int is_special_word(const char *str) {
    for (int i = 0; i < SPECIAL_COUNT; i++) {
        if (strcmp(str, SPECIAL_WORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Получить специальное слово
char* get_special_word(FILE *stream) {
    int first_char = getsym(stream);
    if (first_char == EOF) return NULL;
    
    char single_char[2] = {first_char, '\0'};
    
    // Сначала проверяем односимвольное слово
    if (is_special_word(single_char)) {
        // Пробуем получить двухсимвольное
        int second_char = getsym(stream);
        if (second_char != EOF) {
            char double_char[3] = {first_char, second_char, '\0'};
            if (is_special_word(double_char)) {
                return strdup(double_char);
            } else {
                // Возвращаем второй символ в поток
                buffer_pos--;
            }
        }
        return strdup(single_char);
    }
    
    return NULL;
}

// Разбор строки на слова
StringList* parse_line(FILE *stream) {
    StringList *list = create_list();
    if (!list) return NULL;
    
    int c;
    char *word_buf = malloc(WORD_BUF_INIT_SIZE);
    int buf_size = WORD_BUF_INIT_SIZE;
    int buf_pos = 0;
    
    while ((c = getsym(stream)) != EOF && c != '\n') {
        if (isspace(c)) {
            // Пропускаем пробелы в начале
            if (buf_pos == 0) continue;
            
            // Завершаем текущее слово
            word_buf[buf_pos] = '\0';
            add_string(list, word_buf);
            buf_pos = 0;
            continue;
        }
        
        if (is_special_char(c)) {
            // Если было накоплено обычное слово - добавляем его
            if (buf_pos > 0) {
                word_buf[buf_pos] = '\0';
                add_string(list, word_buf);
                buf_pos = 0;
            }
            
            // Обрабатываем специальное слово
            buffer_pos--; // Возвращаем символ для get_special_word
            char *special = get_special_word(stream);
            if (special) {
                add_string(list, special);
                free(special);
            }
        } else {
            // Обычный символ - добавляем в буфер
            if (buf_pos >= buf_size - 1) {
                buf_size *= 2;
                char *new_buf = realloc(word_buf, buf_size);
                if (!new_buf) {
                    free(word_buf);
                    free_list(list);
                    return NULL;
                }
                word_buf = new_buf;
            }
            word_buf[buf_pos++] = c;
        }
    }
    
    // Добавляем последнее слово, если есть
    if (buf_pos > 0) {
        word_buf[buf_pos] = '\0';
        add_string(list, word_buf);
    }
    
    free(word_buf);
    return list;
}

int main() {
    StringList *list;
    
    printf("Введите строки (Ctrl+D для завершения):\n");
    
    while (!eof_reached) {
        buffer_pos = buffer_size; // Сброс для новой строки
        
        list = parse_line(stdin);
        if (!list || list->size == 0) {
            if (list) free_list(list);
            if (eof_reached) break;
            continue;
        }
        
        printf("Исходный порядок:\n");
        print_list(list);
        
        sort_list(list);
        printf("Лексикографический порядок:\n");
        print_list(list);
        printf("\n");
        
        free_list(list);
    }
    
    return 0;
}


