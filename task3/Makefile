# Компилятор и флаги
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic

# Исходные модули
SRCMODULES = listofwords.c scanner.c
OBJMODULES = $(SRCMODULES:.c=.o)

# Имя исполняемого файла
TARGET = task3

# Основная цель - сборка программы
$(TARGET): main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

# Обобщенное правило для .o файлов
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Автоматическая генерация зависимостей
ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

# Правило для создания файла зависимостей
deps.mk: $(SRCMODULES) main.c
	$(CC) -MM $^ > $@

# Вспомогательные цели
run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f *.o $(TARGET) deps.mk


