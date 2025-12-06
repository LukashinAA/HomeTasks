#ifndef TREE_H
#define TREE_H

#include <setjmp.h>
#include "lexer.h"

typedef enum {NXT, AND, OR} next_type;

typedef struct Tree {
    char **argv;            // список из имени команды и аргументов
    char *infile;           // переназначенный файл стандартного ввода
    char *outfile;          // переназначенный файл стандартного вывода
    int backgrnd;           // =1, если команда подлежит выполнению в фоновом режиме
    next_type type;         // тип связи с следующей командой
    int append;             // флаг дополнения файла (>>)
    struct Tree *pipe;      // следующая команда после "|"
    struct Tree *next;      // следующая после ";" (или после "&")
} tree;

// Глобальные переменные
extern jmp_buf begin;
extern token_list *plst;

// Функции обработки ошибок
void error(char *msg, char *token);

// Функции перенаправления
void in_file(tree *node, char *filename);
void out_file(tree *node, char *filename);
void out_append(tree *node, char *filename);

// Проверка типов токенов
int is_oper(token *t);
int is_next(token *t);
int is_inout(token *t);

// Управление фоновым выполнением
void background(tree *node);

// Инициализация узла
void init_com(tree *node);

// Функции парсера (рекурсивный спуск)
tree *com_sh();
tree *com_list();
tree *conv();
tree *command();
tree *simple_com();

// Основная функция построения дерева
tree *build_tree(token_list *lst);

// Вспомогательные функции
tree *new_tree_node();
void print_struct(tree *node);
void print_tree(tree *node, int level);
void clear_tree(tree *node);

#endif