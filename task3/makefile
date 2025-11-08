# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic

# Исходные модули программы
SRCMODULES = listofwords.c scanner.c
OBJMODULES = $(SRCMODULES:.c=.o)  # Преобразуем .c в .o

# Имя исполняемого файла
TARGET = task3

# Основная цель - сборка программы
$(TARGET): main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

# Обобщенное правило для компиляции .o файлов из .c и .h
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Автоматическая генерация зависимостей между файлами
ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

# Правило для создания файла зависимостей
deps.mk: $(SRCMODULES) main.c
	$(CC) -MM $^ > $@

# Вспомогательные цели (псевдоцели)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладка программы
debug: $(TARGET)
	gdb ./$(TARGET)

# Очистка собранных файлов
clean:
	rm -f *.o $(TARGET) deps.mk

