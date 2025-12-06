#ifndef LEXER_H
#define LEXER_H

// Структура одного токена (лексемы)
typedef struct token {
    char *text;              // текст токена
    struct token *next;      // указатель на следующий токен
} token;

// Структура списка токенов
typedef struct {
    token *first;            // первый токен в списке
    token *last;             // последний токен в списке
} token_list;

// Инициализация пустого списка токенов
void init_token_list(token_list *list);

// Добавление токена в конец списка
void add_token(token_list *list, const char *text);

// Очистка всего списка токенов (освобождение памяти)
void clear_token_list(token_list *list);

// Печать всех токенов списка (для отладки)
void print_tokens(const token_list *list);

// Основная функция лексического анализа
// Принимает строку input, заполняет list токенами
// Возвращает 0 при успехе, -1 при ошибке
int tokenize_input(const char *input, token_list *list);

// Замена переменных окружения ($HOME, $USER, $SHELL, $EUID)
void expand_environment_vars(token_list *list);

#endif
