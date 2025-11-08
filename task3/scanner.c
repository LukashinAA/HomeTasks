#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

/**
 * Проверяет, является ли символ специальным символом командной оболочки
 * @param c Проверяемый символ
 * @return 1 если специальный символ, 0 если нет
 */
int is_special_char(char c) {
    return c == '|' || c == '&' || c == ';' || c == '>' || 
           c == '<' || c == '(' || c == ')' || c == '!';
}

/**
 * Проверяет, может ли символ быть частью слова (команды, имени файла и т.д.)
 * @param c Проверяемый символ
 * @return 1 если символ может быть в слове, 0 если нет
 */
int is_word_char(char c) {
    // Буквы английского алфавита
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    
    // Цифры
    if (c >= '0' && c <= '9') return 1;
    
    // Допустимые специальные символы в словах
    return c == '$' || c == '\'' || c == '`' || c == '/' || c == ',' || c == '_';
}

/**
 * Проверяет, является ли символ пробельным
 * @param c Проверяемый символ
 * @return 1 если пробельный символ, 0 если нет
 */
int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

/**
 * Читает символ из потока с буферизацией для эффективности
 * @param stream Входной поток
 * @param str    Буфер для хранения блока символов
 * @param pos    Указатель на текущую позицию в буфере
 * @param size   Указатель на реальный размер данных в буфере
 * @return       Считанный символ или EOF при конце файла
 */
char getsym(FILE *stream, char *str, int *pos, int *size) {
    // Если буфер пуст или прочитан до конца - читаем новый блок через fscanf
    if (*pos >= *size) {
        // fscanf читает блок символов фиксированной длины
        int result = fscanf(stream, "%255c", str);  // Читаем BLOCK_SIZE-1 символов
        if (result == EOF) {
            return EOF;
        }
        
        // Определяем реальный размер прочитанных данных
        *size = (int)strlen(str);
        *pos = 0;
        
        if (*size == 0) return EOF;
    }
    
    // Возвращаем следующий символ из буфера
    return str[(*pos)++];
}

/**
 * Добавляет текущее накопленное слово в список и сбрасывает буфер
 * @param word_buffer  Указатель на буфер текущего слова
 * @param word_length  Указатель на длину текущего слова
 * @param word_list    Указатель на список слов
 */
void add_current_word(char **word_buffer, int *word_length, StringList *word_list) {
    // Проверяем, есть ли что добавлять
    if (*word_length > 0 && *word_buffer != NULL) {
        (*word_buffer)[*word_length] = '\0';  // Завершаем строку
        add_string(word_list, *word_buffer);  // Добавляем в список
        *word_length = 0;  // Сбрасываем длину для нового слова
    }
}

/**
 * Увеличивает буфер для слова при необходимости
 * @param word_buffer      Указатель на буфер слова
 * @param word_buffer_size Указатель на размер буфера
 * @param word_length      Указатель на текущую длину слова
 * @return 1 при успехе, 0 при ошибке выделения памяти
 */
int increase_word_buffer(char **word_buffer, int *word_buffer_size, int *word_length) {
    // Если буфер еще не выделен - выделяем начальный размер
    if (*word_buffer == NULL) {
        *word_buffer_size = BUFFER_INCREMENT;
        *word_buffer = (char*)malloc(*word_buffer_size);
        return *word_buffer != NULL;  // Возвращаем успешность выделения
    }
    
    // Если буфер заполнен - увеличиваем его
    if (*word_length >= *word_buffer_size - 1) {
        *word_buffer_size += BUFFER_INCREMENT;  // Увеличиваем размер
        char *new_buffer = (char*)realloc(*word_buffer, *word_buffer_size);
        if (new_buffer == NULL) return 0;  // Ошибка перевыделения памяти
        *word_buffer = new_buffer;  // Обновляем указатель на новый буфер
    }
    
    return 1;
}

/**
 * Добавляет символ в буфер текущего слова
 * @param c                Символ для добавления
 * @param word_buffer      Указатель на буфер слова
 * @param word_buffer_size Указатель на размер буфера
 * @param word_length      Указатель на текущую длину слова
 */
void add_char_to_buffer(char c, char **word_buffer, int *word_buffer_size, int *word_length) {
    // Проверяем и увеличиваем буфер при необходимости
    if (!increase_word_buffer(word_buffer, word_buffer_size, word_length)) return;
    
    // Добавляем символ в буфер
    (*word_buffer)[*word_length] = c;
    (*word_length)++;  // Увеличиваем длину слова
}

/**
 * Обрабатывает специальный символ (операторы командной оболочки)
 * @param c           Текущий специальный символ
 * @param stream      Входной поток
 * @param str         Буфер для чтения
 * @param pos         Указатель на позицию в буфере
 * @param size        Указатель на размер буфера
 * @param word_buffer Указатель на буфер текущего слова
 * @param word_length Указатель на длину текущего слова
 * @param word_list   Указатель на список слов
 */
void process_special_char(char c, FILE *stream, char *str, int *pos, int *size, 
                         char **word_buffer, int *word_length, StringList *word_list) {
    // Завершаем текущее слово (если есть)
    add_current_word(word_buffer, word_length, word_list);
    
    // Создаем буфер для специального символа (максимум 2 символа + \0)
    char special_word[3] = {c, '\0', '\0'};
    
    // Смотрим следующий символ для проверки двухсимвольных операторов
    char next_char = getsym(stream, str, pos, size);
    
    if (next_char != EOF) {
        // Проверяем двухсимвольные операторы: ||, &&, >>, <<
        if ((c == '|' && next_char == '|') || (c == '&' && next_char == '&') ||
            (c == '>' && next_char == '>') || (c == '<' && next_char == '<')) {
            special_word[1] = next_char;  // Создаем двухсимвольный оператор
        } else {
            (*pos)--;  // Возвращаем символ обратно в буфер
        }
    }
    
    // Добавляем специальный символ в список как отдельный токен
    add_string(word_list, special_word);
}

/**
 * Основная функция обработки входных данных
 * Читает данные из stdin, разбирает на токены и выводит результаты
 */
void process_input(void) {
    // Буфер для чтения блоков из входного потока
    char read_buffer[BLOCK_SIZE];
    int buffer_pos = 0;    // Текущая позиция в буфере чтения
    int buffer_size = 0;   // Реальный размер данных в буфере чтения
    
    // Буфер для накопления текущего слова
    char *word_buffer = NULL;      // Динамический буфер
    int word_buffer_size = 0;      // Текущий размер буфера слова
    int word_length = 0;           // Текущая длина накопленного слова
    
    // Создаем список для хранения всех токенов
    StringList *word_list = create_string_list();
    if (word_list == NULL) return;  // Ошибка создания списка
    
    char current_char;  // Текущий обрабатываемый символ
    
    // Основной цикл чтения символов
    while ((current_char = getsym(stdin, read_buffer, &buffer_pos, &buffer_size)) != EOF) {
        // Обработка конца строки
        if (current_char == '\n') {
            // Добавляем последнее слово (если есть)
            add_current_word(&word_buffer, &word_length, word_list);
            
            // Если в списке есть токены - обрабатываем их
            if (word_list->size > 0) {
                // Выводим токены в исходном порядке
                print_string_list(word_list);
                
                // Сортируем токены
                sort_string_list(word_list);
                
                // Выводим отсортированные токены
                print_string_list(word_list);
                
                // Очищаем список для следующей строки
                clear_string_list(word_list);
            }
	    fflush(stdout);
            continue;  // Переходим к следующей итерации
        }
        
        // Обработка специальных символов
        if (is_special_char(current_char)) {
            process_special_char(current_char, stdin, read_buffer, &buffer_pos, &buffer_size, &word_buffer, &word_length, word_list);
        }
        // Обработка пробельных символов
        else if (is_whitespace(current_char)) {
            add_current_word(&word_buffer, &word_length, word_list);
        }
        // Обработка символов слов
        else {
            add_char_to_buffer(current_char, &word_buffer, &word_buffer_size, &word_length);
        }
    }
    
    // Обработка конца файла (если ввод завершился без \n)
    add_current_word(&word_buffer, &word_length, word_list);
    if (word_list->size > 0) {
        print_string_list(word_list);
        sort_string_list(word_list);
        print_string_list(word_list);
    }
    
    // Освобождение выделенной памяти
    if (word_buffer != NULL) {
        free(word_buffer);
    }
    destroy_string_list(word_list);
}
