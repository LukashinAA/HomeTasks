#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "lexer.h"
#include "tree.h"
#include "executor.h"

jmp_buf begin;

int main() {
    // Динамический буфер для ввода
    size_t buf_size = 64;
    char *input = malloc(buf_size);
    
    printf("Мой Shell (для выхода введите 'exit')\n");
    
    while (1) {
        printf("myshell> ");
        fflush(stdout);
        
        // Чтение ввода
        int c;
        size_t pos = 0;
        
        while (1) {
            c = getchar();
            if (c == EOF || c == '\n') {
                input[pos] = '\0';
                break;
            }
            
            if (pos >= buf_size - 1) {
                buf_size *= 2;
                input = realloc(input, buf_size);
            }
            
            input[pos++] = c;
        }
        
        // Проверка команд
        if (strcmp(input, "exit") == 0) {
            free(input);
            printf("Выход\n");
            break;
        }
        if (strlen(input) == 0) {
            continue;
        }
        
        // Лексический анализ
        token_list tokens;
        if (tokenize_input(input, &tokens) != 0) {
            printf("Ошибка лексического анализа\n");
            continue;
        }
        
        // Синтаксический анализ
        if (setjmp(begin) == 0) {
            tree *cmd_tree = build_tree(&tokens);
            
            if (cmd_tree) {
                // ИСПОЛНЕНИЕ КОМАНДЫ
                execute_tree(cmd_tree);
                
                // Очистка
                clear_tree(cmd_tree);
            } else {
                printf("Ошибка синтаксиса\n");
            }
        } else {
            printf("Ошибка синтаксиса\n");
        }
        
        clear_token_list(&tokens);
    }
    
    return 0;
}
