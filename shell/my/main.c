#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "lexer.h"
#include "tree.h"

jmp_buf begin;

int main() {
    char input[256];
    
    printf("Введите команду: ");
    fgets(input, sizeof(input), stdin);
    
    // Удаляем символ новой строки
    input[strcspn(input, "\n")] = 0;
    
    // Лексический анализ
    token_list tokens;
    if (tokenize_input(input, &tokens) != 0) {
        printf("Ошибка лексического анализа\n");
        return 1;
    }
    
    // Раскрытие переменных окружения
    expand_environment_vars(&tokens);
    
    // Синтаксический анализ и построение дерева
    if (setjmp(begin) == 0) {
        tree *cmd_tree = build_tree(&tokens);
        if (!cmd_tree) {
            printf("Ошибка синтаксического анализа\n");
            clear_token_list(&tokens);
            return 1;
        }
        
        // Печать структуры дерева
        printf("\nДревовидная структура команды:\n");
        print_struct(cmd_tree);
        
        // Очистка
        clear_tree(cmd_tree);
        clear_token_list(&tokens);
    } else {
        printf("Ошибка синтаксического анализа\n");
        clear_token_list(&tokens);
        return 1;
    }
    
    return 0;
}