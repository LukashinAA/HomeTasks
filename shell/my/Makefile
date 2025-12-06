# Makefile для сборки shell интерпретатора

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Исходные файлы
SRCS = lexer.c tree.c main.c
OBJS = lexer.o tree.o main.o
HDRS = lexer.h tree.h

# Целевой исполняемый файл
TARGET = myshell

# Цель по умолчанию
all: $(TARGET)

# Сборка основного приложения
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Компиляция отдельных модулей
lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c -o $@

tree.o: tree.c tree.h lexer.h
	$(CC) $(CFLAGS) -c tree.c -o $@

main.o: main.c lexer.h tree.h
	$(CC) $(CFLAGS) -c main.c -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Пересборка
rebuild: clean all

# PHONY цели
.PHONY: all clean rebuild
