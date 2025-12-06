#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "tree.h"
#include "lexer.h"


jmp_buf begin;
token_list *plst;
token *current_token;


static token *get_token(void);
static token *peek_token(void);

// Вспомогательные функции
tree *new_tree_node(void) {
    tree *node = (tree *)malloc(sizeof(tree));
    if (!node) return NULL;
    
    node->argv = NULL;
    node->infile = NULL;
    node->outfile = NULL;
    node->backgrnd = 0;
    node->type = NXT;
    node->append = 0;
    node->pipe = NULL;
    node->next = NULL;
    
    return node;
}

void init_com(tree *node) {
    if (!node) return;
    node->argv = NULL;
    node->infile = NULL;
    node->outfile = NULL;
    node->backgrnd = 0;
    node->type = NXT;
    node->append = 0;
    node->pipe = NULL;
    node->next = NULL;
}

void error(char *msg, char *token) {
    fprintf(stderr, "Ошибка: %s. Токен: '%s'\n", msg, token);
    longjmp(begin, 1);
}

// Функции проверки токенов
int is_oper(token *t) {
    if (!t || !t->text) return 0;
    return strcmp(t->text, "|") == 0;
}

int is_next(token *t) {
    if (!t || !t->text) return 0;
    return strcmp(t->text, ";") == 0 || 
           strcmp(t->text, "&") == 0 ||
           strcmp(t->text, "&&") == 0 ||
           strcmp(t->text, "||") == 0;
}

int is_inout(token *t) {
    if (!t || !t->text) return 0;
    return strcmp(t->text, "<") == 0 || 
           strcmp(t->text, ">") == 0 ||
           strcmp(t->text, ">>") == 0;
}

// Функции для установки перенаправлений
void in_file(tree *node, char *filename) {
    if (node && filename) {
        if (node->infile) free(node->infile);
        node->infile = strdup(filename);
    }
}

void out_file(tree *node, char *filename) {
    if (node && filename) {
        if (node->outfile) free(node->outfile);
        node->outfile = strdup(filename);
        node->append = 0;
    }
}

void out_append(tree *node, char *filename) {
    if (node && filename) {
        if (node->outfile) free(node->outfile);
        node->outfile = strdup(filename);
        node->append = 1;
    }
}

void background(tree *node) {
    if (node) node->backgrnd = 1;
}

// Получение текущего токена и переход к следующему
static token *get_token(void) {
    token *t = current_token;
    if (current_token) {
        current_token = current_token->next;
    }
    return t;
}

static token *peek_token(void) {
    return current_token;
}

// simple_com ::= WORD { WORD } { ('<' | '>' | '>>') WORD }
tree *simple_com(void) {
    tree *node = new_tree_node();
    if (!node) return NULL;
    
    init_com(node);
    
    
    char **argv = malloc(sizeof(char *) * 2);
    int argc = 0;
    int argv_size = 2;
    
    if (!argv) {
        free(node);
        return NULL;
    }
    
    argv[0] = NULL;
    
    token *t = peek_token();
    
    // Ожидаем хотя бы одно слово
    if (!t || !t->text || 
        is_oper(t) || is_next(t) || is_inout(t)) {
        free(node);
        free(argv);
        error("Ожидалось имя команды", t ? t->text : "NULL");
        return NULL;
    }
    
    // Собираем аргументы команды
    while (t && t->text && 
           !is_oper(t) && !is_next(t) && !is_inout(t)) {
        
        // Добавляем аргумент в массив
        if (argc >= argv_size - 1) {
            argv_size *= 2;
            char **new_argv = realloc(argv, sizeof(char *) * argv_size);
            if (!new_argv) {
                
                for (int i = 0; i < argc; i++) free(argv[i]);
                free(argv);
                free(node);
                return NULL;
            }
            argv = new_argv;
        }
        argv[argc++] = strdup(t->text);
        argv[argc] = NULL;
        
        get_token(); 
        t = peek_token();
    }
    
    // Обрабатываем перенаправления
    while (t && is_inout(t)) {
        char *op = t->text;
        get_token(); 
        
        token *filename_tok = get_token();
        if (!filename_tok || !filename_tok->text) {
           
            for (int i = 0; i < argc; i++) free(argv[i]);
            free(argv);
            free(node);
            error("Ожидалось имя файла после оператора", op);
            return NULL;
        }
        
        char *filename = filename_tok->text;
        
        if (strcmp(op, "<") == 0) {
            in_file(node, filename);
        } else if (strcmp(op, ">") == 0) {
            out_file(node, filename);
        } else if (strcmp(op, ">>") == 0) {
            out_append(node, filename);
        }
        
        t = peek_token();
    }
    
    
    node->argv = argv;
    
    return node;
}

// command ::= simple_com
tree *command(void) {
    token *t = peek_token();
    
    if (!t) {
        error("Ожидалась команда", "NULL");
        return NULL;
    }
    
    // Только простая команда (скобки убрали)
    return simple_com();
}

// conv ::= command { '|' command }
tree *conv(void) {
    tree *first = command();
    if (!first) return NULL;
    
    token *t = peek_token();
    tree *current = first;
    
    // Обрабатываем конвейер
    while (t && is_oper(t)) {
        get_token(); // потребляем '|'
        
        tree *right = command();
        if (!right) {
            error("Ожидалась команда после '|'", "NULL");
            return first;
        }
        
        // Связываем текущую команду со следующей через pipe
        current->pipe = right;
        current = right; // переходим к следующей команде
        t = peek_token();
    }
    
    return first;
}

// com_list ::= conv { ('&&' | '||') conv }
tree *com_list(void) {
    tree *first = conv();
    if (!first) return NULL;
    
    token *t = peek_token();
    tree *current = first;
    
    while (t && (strcmp(t->text, "&&") == 0 || strcmp(t->text, "||") == 0)) {
        char *op = t->text;
        get_token(); 
        
        tree *right = conv();
        if (!right) {
            error("Ожидалась команда после оператора", op);
            return first;
        }
        
        current->next = right;
        if (strcmp(op, "&&") == 0) {
            current->type = AND;
        } else if (strcmp(op, "||") == 0) {
            current->type = OR;
        }
        
        current = right; // переходим к следующей команде
        t = peek_token();
    }
    
    return first;
}

// com_sh ::= com_list ['&'] | com_list [';' com_sh]
tree *com_sh(void) {
    tree *node = com_list();
    if (!node) return NULL;
    
    token *t = peek_token();
    
    if (!t) return node;
    
    // Проверяем фон или точку с запятой
    if (strcmp(t->text, "&") == 0) {
        get_token(); // потребляем '&'
        background(node);
        
        // Проверяем, есть ли еще команды после &
        t = peek_token();
        if (t && strcmp(t->text, ";") == 0) {
            get_token(); // потребляем ';'
            tree *next_cmd = com_sh();
            if (next_cmd) {
                tree *current = node;
                while (current->next) {
                    current = current->next;
                }
                current->next = next_cmd;
                current->type = NXT;
            }
        }
    } else if (strcmp(t->text, ";") == 0) {
        get_token(); // потребляем ';'
        tree *next_cmd = com_sh();
        if (next_cmd) {
            tree *current = node;
            while (current->next) {
                current = current->next;
            }
            current->next = next_cmd;
            current->type = NXT;
        }
    }
    
    return node;
}

// Основная функция построения дерева из списка токенов
tree *build_tree(token_list *lst) {
    if (!lst || !lst->first) return NULL;
    
    plst = lst;
    current_token = lst->first;
    
    // Используем setjmp для обработки ошибок
    if (setjmp(begin) == 0) {
        return com_sh();
    } else {
        return NULL;
    }
}

// Функция печати дерева (для отладки)
void print_tree(tree *node, int level) {
    if (!node) return;
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("Команда на уровне %d:\n", level);
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Аргументы: ");
    if (node->argv) {
        for (int i = 0; node->argv[i]; i++) {
            printf("%s ", node->argv[i]);
        }
    }
    printf("\n");
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Входной файл: %s\n", node->infile ? node->infile : "(нет)");
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Выходной файл: %s (append=%d)\n", 
           node->outfile ? node->outfile : "(нет)", node->append);
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Фон: %d\n", node->backgrnd);
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("  Тип связи: ");
    switch(node->type) {
        case NXT: printf("NXT (;)\n"); break;
        case AND: printf("AND (&&)\n"); break;
        case OR: printf("OR (||)\n"); break;
    }
    
    if (node->pipe) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("  Следующая в конвейере (|):\n");
        print_tree(node->pipe, level + 1);
    }
    
    if (node->next) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("  Следующая команда (тип: ");
        switch(node->type) {
            case NXT: printf(";"); break;
            case AND: printf("&&"); break;
            case OR: printf("||"); break;
        }
        printf("):\n");
        print_tree(node->next, level + 1);
    }
}

void print_struct(tree *node) {
    printf("\n=== Структура команды ===\n");
    print_tree(node, 0);
    printf("=========================\n");
}

// Освобождение памяти дерева
void clear_tree(tree *node) {
    if (!node) return;
    
    // Рекурсивно освобождаем связанные узлы
    if (node->pipe) clear_tree(node->pipe);
    if (node->next) clear_tree(node->next);
    
    // Освобождаем аргументы
    if (node->argv) {
        for (int i = 0; node->argv[i]; i++) {
            free(node->argv[i]);
        }
        free(node->argv);
    }
    
    // Освобождаем имена файлов
    if (node->infile) free(node->infile);
    if (node->outfile) free(node->outfile);
    
    // Освобождаем сам узел
    free(node);
}