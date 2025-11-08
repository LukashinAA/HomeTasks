#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

int is_special_char(char c) {
    return c == '|' || c == '&' || c == ';' || c == '>' || 
           c == '<' || c == '(' || c == ')' || c == '!';
}

int is_word_char(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= '0' && c <= '9') return 1;
    return c == '$' || c == '\'' || c == '`' || c == '/' || c == ',' || c == '_';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

char getsym(FILE *stream, char *str, int *pos, int *size) {
    if (*pos >= *size) {
        int result = fscanf(stream, "%255c", str);
        if (result == EOF) {
            return EOF;
        }
        
        *size = (int)strlen(str);
        *pos = 0;
        
        if (*size == 0) return EOF;
    }
    
    return str[(*pos)++];
}

void add_current_word(char **word_buffer, int *word_length, StringList *word_list) {
    if (*word_length > 0 && *word_buffer != NULL) {
        (*word_buffer)[*word_length] = '\0';
        add_string(word_list, *word_buffer);
        *word_length = 0;
    }
}

int increase_word_buffer(char **word_buffer, int *word_buffer_size, int *word_length) {
    if (*word_buffer == NULL) {
        *word_buffer_size = BUFFER_INCREMENT;
        *word_buffer = (char*)malloc(*word_buffer_size);
        return *word_buffer != NULL;
    }
    
    if (*word_length >= *word_buffer_size - 1) {
        *word_buffer_size += BUFFER_INCREMENT;
        char *new_buffer = (char*)realloc(*word_buffer, *word_buffer_size);
        if (new_buffer == NULL) return 0;
        *word_buffer = new_buffer;
    }
    
    return 1;
}

void add_char_to_buffer(char c, char **word_buffer, int *word_buffer_size, int *word_length) {
    if (!increase_word_buffer(word_buffer, word_buffer_size, word_length)) return;
    (*word_buffer)[*word_length] = c;
    (*word_length)++;
}

void process_special_char(char c, FILE *stream, char *str, int *pos, int *size, 
                         char **word_buffer, int *word_length, StringList *word_list) {
    add_current_word(word_buffer, word_length, word_list);
    
    char special_word[3] = {c, '\0', '\0'};
    char next_char = getsym(stream, str, pos, size);
    
    if (next_char != EOF) {
        if ((c == '|' && next_char == '|') || (c == '&' && next_char == '&') ||
            (c == '>' && next_char == '>') || (c == '<' && next_char == '<')) {
            special_word[1] = next_char;
        } else {
            (*pos)--;
        }
    }
    
    add_string(word_list, special_word);
}

void process_input(void) {
    char read_buffer[BLOCK_SIZE];
    int buffer_pos = 0;
    int buffer_size = 0;
    
    char *word_buffer = NULL;
    int word_buffer_size = 0;
    int word_length = 0;
    
    StringList *word_list = create_string_list();
    if (word_list == NULL) return;
    
    char current_char;
    
    while ((current_char = getsym(stdin, read_buffer, &buffer_pos, &buffer_size)) != EOF) {
        if (current_char == '\n') {
            add_current_word(&word_buffer, &word_length, word_list);
            
            if (word_list->size > 0) {
                print_string_list(word_list);
                sort_string_list(word_list);
                print_string_list(word_list);
                clear_string_list(word_list);
            }
            fflush(stdout);
            continue;
        }
        
        if (is_special_char(current_char)) {
            process_special_char(current_char, stdin, read_buffer, &buffer_pos, &buffer_size,
                               &word_buffer, &word_length, word_list);
        }
        else if (is_whitespace(current_char)) {
            add_current_word(&word_buffer, &word_length, word_list);
        }
        else {
            add_char_to_buffer(current_char, &word_buffer, &word_buffer_size, &word_length);
        }
    }
    
    add_current_word(&word_buffer, &word_length, word_list);
    if (word_list->size > 0) {
        print_string_list(word_list);
        sort_string_list(word_list);
        print_string_list(word_list);
    }
    
    if (word_buffer != NULL) {
        free(word_buffer);
    }
    destroy_string_list(word_list);
}